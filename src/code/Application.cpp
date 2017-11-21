#include "Application.h"
#include "Script.h"
//#include <vector>
//#include <string>
//#include <SFML/graphics.hpp>
//#include "json\json.hpp"
#include "JsonHelpers.h"

Application::Application()
{
	rapidjson::Document document = JsonHelpers::CreateOrOpen(L"bajs.json", applicationData.GetDefaultData());

	applicationData.Serialize(document);
}

Application::~Application()
{
}

void Application::loadData()
{

}