// by Dmitry Kolontay

#include <vector>
import <iostream>;
import Resources;

//#define COMMANDS_COUNT_INIT 0

//#define COMMANDS_COUNT COMMANDS_COUNT_INIT+1
//#define COMMANDS_COUNT COMMANDS_COUNT_INIT+2
//#redefine COMMANDS_COUNT COMMANDS_COUNT_INIT+3

int main()
{
    //ResourceContainerD<ResourceType::Heat> container;

    ResoursePack<ResourceType::Heat, ResourceType::Conductor> test( 1.f, 2.f );

    ResourceInfo<ResourceType::Heat> testInfo{ 5.f };
    //std::vector<int> test
    ResourceContainerD testContainer{ testInfo, 11.f };

    //constexpr auto testRT = static_cast<ResourceType>(90);

    //ResourceInfo<testRT> testInfo2{ 5.f };
    std::cout << test.GetResourceAmount<ResourceType::Conductor>() << std::endl;
    std::cout << "Hello World!\n";
}