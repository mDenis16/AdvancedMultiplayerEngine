#include "pch.h"
#include <iostream>
#include <EngineSimulator\SceneView.hpp>


#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"

int main()
{
  
    Multiplayer.Connect("192.168.88.252", 5555);
   
    /*SceneView scene;
    scene.Init();*/

    while (true) {

    }

    std::cout << "Hello World!\n";
}
