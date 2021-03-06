#include "c++/methods/callbacks/AuthenticationInstanceCallbacks.h"
#include "c++/schemas/Response.h"

namespace modio
{
std::map<u32, GenericCall *> email_request_calls;
std::map<u32, GenericCall *> email_exchange_calls;

void onEmailRequest(void *object, ModioResponse modio_response)
{
  u32 call_id = (u32)((uintptr_t)object);

  modio::Response response;

  response.initialize(modio_response);

  email_request_calls[call_id]->callback(response);

  delete email_request_calls[call_id];
  email_request_calls.erase(call_id);
}

void onEmailExchange(void *object, ModioResponse modio_response)
{
  u32 call_id = (u32)((uintptr_t)object);

  modio::Response response;
  response.initialize(modio_response);

  email_exchange_calls[call_id]->callback(response);

  delete email_exchange_calls[call_id];
  email_exchange_calls.erase(call_id);
}

void clearAuthenticationRequestCalls()
{
  for (auto email_request_call : email_request_calls)
    delete email_request_call.second;
  email_request_calls.clear();

  for (auto email_exchange_call : email_exchange_calls)
    delete email_exchange_call.second;
  email_exchange_calls.clear();
}
} // namespace modio
