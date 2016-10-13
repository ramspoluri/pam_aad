#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include "json.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include "clientConfig.h"
#include "SimpleIni.h"
using namespace std;

string getClientRequestId()
{
  string client_request_id = "filler";
  return client_request_id;
}

string  getDeviceCode()
{
  string client_id = "REDACTED";
  string resource = "00000002-0000-0000-c000-000000000000";
  string tenant = "REDACTED";
  string client_request_id = getClientRequestId();
  string url = "https://login.microsoftonline.com/" + tenant + "/oauth2/devicecode?resource=" + resource + "&client_id=" + client_id;
  RestClient::Response r = RestClient::get(url);
  if (r.code == 200)
  {
    cout << "\nSuccessful first request!\n";
    cout << r.body + "\n";
    return r.body;
  }
  else
  {
    cout << "Error of some kind, non 200 response.\n";
    cout <<"\nerror is: " + r.body + "\n";
    cout <<"\nAnd the code is:";
    cout << r.code;
    return r.body;
  }
}

string getUriForPolling(string rawMessage){
  string poll_uri = "filler";
  return poll_uri;
}

string pollForToken(string poll_uri){
  RestClient::Response r = RestClient::post(poll_uri, "text/json", "{\"foo\": \"bla\"}");
  return r.body;
}

bool providedToken(string response_body){
  return false;
}

int AuthenticateToMicrosoft(){
  cout << "Welcome!\n";
  string deviceCodeMessage = getDeviceCode();
  string parsedUri = getUriForPolling(deviceCodeMessage);
  cout << "\n" + deviceCodeMessage;
  bool gotToken = false;
  string response;
  while (gotToken){
    sleep(5000); //wait for 5 seconds before polling
    response = pollForToken(parsedUri);
    gotToken = providedToken(response);
  } 

}

int main(int args, char* argv[])
{
  CSimpleIniA ini;
  ini.SetUnicode();
  ini.LoadFile("/etc/security/oauth.config.ini");
  string *tenant;
  string *client_id;
  string *resource_id;
  string cfgfilepath = "/etc/security/oauth.config.ini";
  ClientConfig::readConfigFile(cfgfilepath, tenant, client_id, resource_id);
  int response;
  string username; 
  string password;
  response = AuthenticateToMicrosoft(); 
  return response;
}
