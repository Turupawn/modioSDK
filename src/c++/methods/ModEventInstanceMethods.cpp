#include "c++/ModIOInstance.h"

namespace modio
{
  void Instance::getAllModEvents(modio::FilterCreator& filter, const std::function<void(const modio::Response&, const std::vector<modio::ModEvent> & mod_events)>& callback)
  {
    const struct GetAllModEventsCall* get_all_mod_events_call = new GetAllModEventsCall{callback};
    get_all_mod_events_calls[this->current_call_id] = (GetAllModEventsCall*)get_all_mod_events_call;

    modioGetAllModEvents((void*)new u32(this->current_call_id), *filter.getFilter(), &onGetAllModEvents);

    this->current_call_id++;
  }

  void Instance::listendToModEvents(const std::function<void(const modio::Response&, const std::vector<modio::ModEvent> & mod_events)>& callback)
  {
    listend_to_mod_event_call = new ListendToModEventsCall{callback};
    modioListendToEvents(&onListendToModEvents);
  }
}