#include "c/methods/ModfileMethods.h"

extern "C"
{
  void modioGetModfile(void* object, u32 mod_id, u32 modfile_id, void (*callback)(void* object, ModioResponse response, ModioModfile modfile))
  {
    std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/files/" + modio::toString(modfile_id) + "?api_key=" + modio::API_KEY;

    u32 call_number = modio::curlwrapper::getCallNumber();

    get_modfile_callbacks[call_number] = new GetModfileParams;
    get_modfile_callbacks[call_number]->callback = callback;
    get_modfile_callbacks[call_number]->object = object;

    modio::curlwrapper::get(call_number, url, modio::getHeaders(), &modioOnGetModfile);
  }

  void modioGetModfiles(void* object, u32 mod_id, ModioFilterCreator filter, void (*callback)(void* object, ModioResponse response, ModioModfile modfiles[], u32 modfiles_size))
  {
    std::string filter_string = modio::getFilterString(&filter);
    std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/files?" + filter_string + "&api_key=" + modio::API_KEY;

    u32 call_number = modio::curlwrapper::getCallNumber();

    get_modfiles_callbacks[call_number] = new GetModfilesParams;
    get_modfiles_callbacks[call_number]->callback = callback;
    get_modfiles_callbacks[call_number]->object = object;
    get_modfiles_callbacks[call_number]->url = url;
    get_modfiles_callbacks[call_number]->is_cache = false;

    std::string cache_filename = modio::getCallFileFromCache(url, filter.cache_max_age_seconds);
    if(cache_filename != "")
    {
      json cache_file_json = modio::openJson(modio::getModIODirectory() + "cache/" + cache_filename);

      if(!cache_file_json.empty())
      {
        get_modfiles_callbacks[call_number]->is_cache = true;
        modioOnGetModfiles(call_number, 200, cache_file_json);
        return;
      }
    }

    modio::curlwrapper::get(call_number, url, modio::getHeaders(), &modioOnGetModfiles);
  }

  void modioAddModfile(u32 mod_id, ModioModfileCreator modfile_creator)
  {
    modio::curlwrapper::queueModfileUpload(mod_id, &modfile_creator);
  }
/*
  void modioAddModfile(void* object, u32 mod_id, ModioModfileCreator modfile_creator, void (*callback)(void* object, ModioResponse response, ModioModfile modfile))
  {
    modio::minizipwrapper::compress(modfile_creator.path, modio::getModIODirectory() + "tmp/modfile.zip");
    std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/files";

    u32 call_number = modio::curlwrapper::getCallNumber();

    add_modfile_callbacks[call_number] = new AddModfileParams;
    add_modfile_callbacks[call_number]->callback = callback;
    add_modfile_callbacks[call_number]->object = object;

    std::map<std::string, std::string> curlform_files;
    curlform_files["filedata"] = modio::getModIODirectory() + "tmp/modfile.zip";

    modio::curlwrapper::postForm(call_number, url, modio::getHeaders(), modio::convertModfileCreatorToMultimap(&modfile_creator), curlform_files, &modioOnModfileAdded);
  }
  */

  void modioEditModfile(void* object, u32 mod_id, u32 modfile_id, ModioModfileEditor modfile_editor, void (*callback)(void* object, ModioResponse response, ModioModfile modfile))
  {
    u32 call_number = modio::curlwrapper::getCallNumber();

    edit_modfile_callbacks[call_number] = new EditModfileParams;
    edit_modfile_callbacks[call_number]->modfile_id = modfile_id;
    edit_modfile_callbacks[call_number]->callback = callback;
    edit_modfile_callbacks[call_number]->object = object;

    std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/files/" + modio::toString(modfile_id);

    std::multimap<std::string,std::string> modfile_params = modio::convertModfileEditorToMultimap(&modfile_editor);
    for(std::multimap<std::string,std::string>::iterator i = modfile_params.begin(); i != modfile_params.end(); i++)
    {
      if(i==modfile_params.begin())
        url+="?";
      else
        url+="&";
      url+=(*i).first + "=" + (*i).second;
    }

    modio::curlwrapper::put(call_number, url, modio::getHeaders(), modio::convertModfileEditorToMultimap(&modfile_editor), &modioOnModfileEdited);
  }
}
