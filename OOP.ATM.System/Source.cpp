#include <iostream>
#include "clsLoginScreen.h"

int main()
{
    while (true)
    {
        if (!clsLoginScreen::ShowLoginScreen())
            break;
    }
    cout << "\nProgram will now exit.\n";
    return 0;
}