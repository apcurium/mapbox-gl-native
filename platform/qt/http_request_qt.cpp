#include "http_request_qt.hpp"

#include "http_context_qt.hpp"

#include <mbgl/storage/response.hpp>
#include <mbgl/util/parsedate.h>
#include <mbgl/util/string.hpp>
#include <mbgl/util/time.hpp>

#include <QByteArray>
#include <QNetworkReply>
#include <QPair>

namespace mbgl {

HTTPQtRequest::HTTPQtRequest(HTTPQtContext* context, const Resource& resource_,
    Callback callback, std::shared_ptr<const Response> response)
    : HTTPRequestBase(resource_, callback)
    , m_context(context)
    , m_existingResponse(response)
{
    m_context->request(this);
}

QUrl HTTPQtRequest::url() const
{
    return QUrl::fromPercentEncoding(QByteArray(resource.url.data(), resource.url.size()));
}

QNetworkRequest HTTPQtRequest::networkRequest() const
{
    QNetworkRequest req = QNetworkRequest(url());
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    req.setRawHeader("User-Agent", "MapboxGL/1.0 [Qt]");

    if (!etag().isEmpty()) {
        req.setRawHeader("If-None-Match", etag().toAscii());
    } else if (modified().isEmpty()) {
        req.setRawHeader("If-Modified-Since", modified().toAscii());
    }

    return req;
}

void HTTPQtRequest::handleNetworkReply(QNetworkReply *reply)
{
    std::shared_ptr<mbgl::Response> response = std::make_shared<mbgl::Response>();

    // Handle non-HTTP errors (i.e. like connection).
    if (reply->error() && reply->error() < 100) {
        response->message = reply->errorString().toStdString();
        notify(std::move(response), FileCache::Hint::No);
        delete this;

        return;
    }

    QPair<QByteArray, QByteArray> line;
    foreach(line, reply->rawHeaderPairs()) {
        QString header = QString(line.first).toLower();

        if (header == "last-modified") {
            response->modified = parse_date(line.second.constData());
        } else if (header == "etag") {
            response->etag = std::string(line.second.constData(), line.second.size());
        } else if (header == "cache-control") {
            response->expires = parseCacheControl(line.second.constData());
        } else if (header == "expires") {
            response->expires = parse_date(line.second.constData());
        }
    }

    QByteArray bytes = reply->readAll();
    if (bytes.size()) {
        response->data = std::make_shared<std::string>(bytes.data(), bytes.size());
    }

    FileCache::Hint cacheHint = FileCache::Hint::Full;
    int responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch(responseCode) {
    case 0: // Asset.
        response->status = Response::Successful;
        cacheHint = FileCache::Hint::No;
        break;
    case 304:
        if (!m_existingResponse) {
            response->status = Response::Successful;
            break;
        }

        response->status = m_existingResponse->status;
        response->message = m_existingResponse->message;
        response->modified = m_existingResponse->modified;
        response->etag = m_existingResponse->etag;
        response->data = m_existingResponse->data;

        cacheHint = FileCache::Hint::Refresh;
        break;
    case 200:
        response->status = Response::Successful;
        break;
    case 404:
        response->status = Response::NotFound;
        break;
    default:
        response->message = "HTTP status code " + util::toString(responseCode);
    }

    notify(std::move(response), cacheHint);
    delete this;
}

void HTTPQtRequest::retry(uint64_t)
{
    // TODO: Not implemented.
}

void HTTPQtRequest::retry()
{
    // TODO: Not implemented.
}

void HTTPQtRequest::cancel()
{
    m_context->cancel(this);
    delete this;
}

QString HTTPQtRequest::etag() const
{
    if (!m_existingResponse) {
        return QString();
    }

    return m_existingResponse->etag.c_str();
}

QString HTTPQtRequest::modified() const
{
    if (!m_existingResponse) {
        return QString();
    }

    return util::rfc1123(m_existingResponse->modified).c_str();
}

} // namespace mbgl
