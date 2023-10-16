// by Dmitry Kolontay

import <iostream>;
import Resources;

int main()
{
    //ResourceContainerD<ResourceType::Heat> container;

    ResoursePack<ResourceType::Heat, ResourceType::Conductor> test( 1.f, 2.f );

    ResourceInfo<ResourceType::Heat> testInfo{ 5.f };
    ResourceContainerD testContainer{ testInfo, 11.f };
    std::cout << test.GetResourceAmount<ResourceType::Conductor>() << std::endl;
    std::cout << "Hello World!\n";
}