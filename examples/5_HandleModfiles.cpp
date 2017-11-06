#include "ModIOSDK.h"

bool mods_get_finished = false;
bool modfile_installed = false;
bool modfile_edited = false;

ModioModfile* global_modfile = NULL;

void onModfileInstalled(ModioResponse* response, char* path)
{
  cout<<"Install Mod response: "<<response->code<<endl;
  if(response->code == 200)
  {
    cout<<"Mod installed successfully!"<<endl;
  }
  modfile_installed = true;
}

void onModfileEdited(ModioResponse* response, ModioModfile* modfile)
{
  cout<<"Edit Mod response: "<<response->code<<endl;
  if(response->code == 200)
  {
    cout<<"Mod edited successfully!"<<endl;
  }
  modfile_edited = true;
}

void onModsGet(ModioResponse* response, ModioMod* mods, int mods_size)
{
  cout<<"GetMods response: "<<response->code<<endl;
  if(response->code == 200)
  {
    cout<<"Listing mods"<<endl;
    cout<<"============"<<endl;
    for(int i=0;i<(int)mods_size;i++)
    {
      cout<<"Mod["<<i<<"]"<<endl;
      cout<<"Id: \t"<<mods[i].id<<endl;
      cout<<"Name:\t"<<mods[i].name<<endl;
      cout<<"Installing..."<<endl;

      global_modfile = mods[i].modfile;
    }
  }

  mods_get_finished = true;
}

int main(void)
{
  modioInit(7, (char*)"e91c01b8882f4affeddd56c96111977b");

  ModioFilter* filter = new ModioFilter;
  modioInitFilter(filter);
  modioSetFilterLimit(filter,1);

  cout<<"Getting mods..."<<endl;
  modioGetMods(filter, &onModsGet);

  while(!mods_get_finished);

  cout<<"Editing modfile..."<<endl;

  ModioModfileHandler* modfile_handler = new ModioModfileHandler;
  modioInitModfileHandler(modfile_handler);
  modioSetModfileActive(modfile_handler,false);
  modioSetModfileChangelog(modfile_handler,(char*)"Stuff was changed on this mod via the examples.");

  modioEditModfile(global_modfile, modfile_handler, &onModfileEdited);

  while(!modfile_edited);

  cout<<"Installing modfile..."<<endl;

  string instalation_path_str = string("mods_dir/modfile_") + modio::toString(global_modfile->id);
  char* instalation_path = new char[instalation_path_str.size() + 1];
  strcpy(instalation_path, instalation_path_str.c_str());
  modioInstallModfile(global_modfile, instalation_path, &onModfileInstalled);

  while(!modfile_installed);

  modioShutdown();

  cout<<"Process finished"<<endl;

  return 0;
}
