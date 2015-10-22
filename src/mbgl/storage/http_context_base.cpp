#include <mbgl/storage/http_context_base.hpp>

#include <assert.h>

namespace mbgl {

HTTPContextBase::HTTPContextBase()
    : reachability([this] { retryRequests(); }) {
    NetworkStatus::Subscribe(&reachability);
    reachability.unref();
}

HTTPContextBase::~HTTPContextBase() {
    assert(requests.empty());
    NetworkStatus::Unsubscribe(&reachability);
}

void HTTPContextBase::addRequest(HTTPRequestBase* request) {
    requests.insert(request);
}

void HTTPContextBase::removeRequest(HTTPRequestBase* request) {
    requests.erase(request);
}

void HTTPContextBase::retryRequests() {
    for (auto request : requests) {
        request->retry();
    }
}

}
