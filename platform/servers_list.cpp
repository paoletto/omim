#include "platform/servers_list.hpp"
#include "platform/http_request.hpp"
#include "platform/settings.hpp"
#include "platform/platform.hpp"

#include "base/logging.hpp"
#include "base/assert.hpp"

#include "3party/jansson/myjansson.hpp"


#define SETTINGS_SERVERS_KEY "LastBaseUrls"


namespace downloader
{

bool ParseServerList(string const & jsonStr, vector<string> & outUrls)
{
  outUrls.clear();
  try
  {
    my::Json root(jsonStr.c_str());
    for (size_t i = 0; i < json_array_size(root.get()); ++i)
    {
      char const * url = json_string_value(json_array_get(root.get(), i));
      if (url)
        outUrls.push_back(url);
    }
  }
  catch (my::Json::Exception const & ex)
  {
    LOG(LERROR, ("Can't parse server list json:", ex.Msg(), jsonStr));
  }
  return !outUrls.empty();
}

void GetServerListFromRequest(HttpRequest const & request, vector<string> & urls)
{
  if (request.Status() == HttpRequest::ECompleted &&
      ParseServerList(request.Data(), urls))
  {
    Settings::Set(SETTINGS_SERVERS_KEY, request.Data());
  }
  else
  {
    LOG(LWARNING, ("Can't get servers list from request"));

    string serverList;
    if (!Settings::Get(SETTINGS_SERVERS_KEY, serverList))
      serverList = GetPlatform().DefaultUrlsJSON();
    VERIFY ( ParseServerList(serverList, urls), () );
  }
}

}
