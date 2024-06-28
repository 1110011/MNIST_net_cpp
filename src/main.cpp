#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int menuIndex = -1;

#include "winGUI.h"

#include <omp.h>
int maxThreads = omp_get_max_threads();

#include "network.h"


std::vector<std::pair<std::vector<float>,std::vector<float>>> loadTrainingData(void)
{
    unsigned int numLbls;

    // find file data resource
    HRSRC fdRes = FindResourceA(NULL, MAKEINTRESOURCE(6), MAKEINTRESOURCE(256));
    // get file size
    size_t fdRSize = (size_t)SizeofResource(NULL, fdRes);
    // get resource pointer
    HGLOBAL fdRp = LoadResource(NULL, fdRes);
    unsigned int fdRpOf = 0;

    std::string line;

    line = resourceGetLine(fdRpOf, fdRp, fdRSize);
    numLbls = stoi(line);

    std::vector<std::pair<std::vector<float>,std::vector<float>>> trData;
    trData.resize(numLbls);

    for(unsigned int i = 0; i < numLbls; ++i)
    {
        line = resourceGetLine(fdRpOf, fdRp, fdRSize);
        unsigned int val = stoi(line);
        std::vector<float> outTmp;
        outTmp.resize(10);
        for(unsigned int j = 0; j < 10; ++j)
            outTmp[j] = (j==val?1.0f:0.0f);

        trData[i].second = outTmp;
    }

    std::cout << "Training labels loaded successfully\n";

    // find file data resource
    fdRes = FindResourceA(NULL, MAKEINTRESOURCE(7), MAKEINTRESOURCE(256));
    // get file size
    fdRSize = (size_t)SizeofResource(NULL, fdRes);
    // get resource pointer
    fdRp = LoadResource(NULL, fdRes);
    fdRpOf = 0;

    for(unsigned int i = 0; i < numLbls; ++i)
    {
        trData[i].first.resize(28*28);

        for(unsigned int j = 0; j < 28*28; ++j)
        {
            line = resourceGetLine(fdRpOf, fdRp, fdRSize);
            trData[i].first[j] = (float)stoi(line)/255.0f;
        }
    }

    std::cout << "Training images loaded successfully\n";

    return trData;
}

float brushFalloff(const float a)
{
    return a*a;
}

int main()
{
    srand(time(0));

    bool training_data_loaded = false;
    Network net = Network({784, 30, 10});
    //net.loadFromCompactBinaryFile("model.bnf");
    net.loadFromBNFAsResource(5);

    std::vector<std::pair<std::vector<float>,std::vector<float>>> training_data;

    //training_data = loadTrainingData();
    //training_data_loaded = true;

    /*std::cout << net.evaluate(training_data,10000) << "/10000 correct\n";
    net.SGD(training_data, 2, 20, 0.5, 6.0);
    std::cout << net.evaluate(training_data,10000) << "/10000 correct\n";

    net.saveToCompactBinaryFile("model.bnf");*/

    // interactive demo

    Window mainWindow = Window("MNIST", 560, 560);

    HMENU hmenu = CreateMenu();

    MENUITEMINFOA mItem;
    mItem.cbSize = sizeof(MENUITEMINFOA);
    mItem.fMask = MIIM_TYPE | MIIM_ID;
    mItem.fType = MFT_STRING;
    mItem.wID = 0;
    std::string mItemText = "Load MNIST data-set";
    mItem.dwTypeData = &mItemText[0];
    mItem.cch = mItemText.size();

    InsertMenuItemA(hmenu, 0, TRUE, &mItem);

    mItem.wID = 1;
    mItemText = "Help";
    mItem.dwTypeData = &mItemText[0];
    mItem.cch = mItemText.size();

    InsertMenuItemA(hmenu, 1, TRUE, &mItem);

    SetMenu(mainWindow.hwnd, hmenu);

    shapeRenderer shpRender = shapeRenderer(3,4);

    std::vector<float> inputActivation;
    inputActivation.resize(28*28);
    std::vector<float> outActivation;

    for(unsigned int i = 0; i < inputActivation.size(); ++i)
        inputActivation[i] = 0.0f;

    bool rDown = false;
    bool eDown = false;
    bool zDown = false;
    bool cDown = false;

    float brushSize = 3/28.0f;

    while(mainWindow.update())
    {

        switch(menuIndex)
        {
        case 0:
            if(!training_data_loaded)
            {
                training_data = loadTrainingData();
                training_data_loaded = true;
            }
            else std::cout << "MNIST data-set already loaded.\n";
            menuIndex = -1;
            break;

        case 1:
            std::cout << "Use the left mouse button to draw on the canvas and the right mouse button to erase.\n";
            std::cout << "Press E to run the network on the current canvas image.\n";
            std::cout << "Press C to center the image in the canvas (helps with classification accuracy).\n";
            std::cout << "Press R to load a random image from the MNIST data-set (only works if the data-set has been loaded using the appropriate button in the menu-bar).\n";
            std::cout << "Press Z to clear the canvas.\n";
            std::cout << "The blue circle represents the \"center of mass\" of the canvas pixels (it calculates the average of the positions of all pixels with a brightness of over 0.5).\n";
            menuIndex = -1;
            break;

        case -1:
        default:
            break;
        }

        if(training_data_loaded)
        {
            if(GetAsyncKeyState('R')) // load random training instance
            {
                if(!rDown)
                {
                    rDown = true;
                    unsigned int tmp = rand()%training_data.size();
                    inputActivation = training_data[tmp].first;
                    for(unsigned int i = 0; i < 10; ++i)
                    {
                        if(training_data[tmp].second[i] > 0.5f)
                            std::cout << "Loading validation instance number " << tmp << ". Label: " << i << '\n';
                    }
                }
            }
            else rDown = false;
        }

        if(GetAsyncKeyState('E')) // evaluate using network
        {
            if(!eDown)
            {
                eDown = true;
                outActivation = net.feedForward_softmax(inputActivation, 1.0f);

                unsigned int maxIndex = 0;
                float maxVal = outActivation[0];
                std::cout << "0: " << outActivation[0] << '\n';
                for(unsigned int i = 1; i < outActivation.size(); ++i)
                {
                    std::cout << i << ": " << outActivation[i] << '\n';

                    if(outActivation[i] > maxVal)
                    {
                        maxVal = outActivation[i];
                        maxIndex = i;
                    }
                }
                std::cout << "Network guesses " << maxIndex << " with confidence " << maxVal << '\n';
            }
        }
        else eDown = false;

        if(GetAsyncKeyState('Z')) // zero input activation
        {
            if(!zDown)
            {
                zDown = true;
                for(unsigned int i = 0; i < inputActivation.size(); ++i)
                    inputActivation[i] = 0.0f;
            }
        }
        else zDown = false;

        if(GetAsyncKeyState(VK_LBUTTON))
        {
            if(mainWindow.cursorNDC.x < 1.0f && mainWindow.cursorNDC.x > -1.0f && mainWindow.cursorNDC.y < 1.0f && mainWindow.cursorNDC.y > -1.0f) // cursor in bounds
            {
                for(unsigned int y = 0; y < 28; ++y)
                {
                    for(unsigned int x = 0; x < 28; ++x)
                    {
                        vec2 center = vec2(-1.0f+(float)x*(2.0f/28.0f), 1.0f-(float)y*(2.0f/28.0f));
                        vec2 diff = mainWindow.cursorNDC-center;

                        if(diff.mag() < brushSize)
                            inputActivation[y*28+x] = std::clamp(inputActivation[y*28+x]+brushFalloff((brushSize-diff.mag())/brushSize)*2, 0.0f, 1.0f);
                    }
                }
            }
        }

        if(GetAsyncKeyState(VK_RBUTTON))
        {
            if(mainWindow.cursorNDC.x < 1.0f && mainWindow.cursorNDC.x > -1.0f && mainWindow.cursorNDC.y < 1.0f && mainWindow.cursorNDC.y > -1.0f) // cursor in bounds
            {
                for(unsigned int y = 0; y < 28; ++y)
                {
                    for(unsigned int x = 0; x < 28; ++x)
                    {
                        vec2 center = vec2(-1.0f+(float)x*(2.0f/28.0f), 1.0f-(float)y*(2.0f/28.0f));
                        vec2 diff = mainWindow.cursorNDC-center;

                        if(diff.mag() < brushSize)
                            inputActivation[y*28+x] = std::clamp(inputActivation[y*28+x]-brushFalloff((brushSize-diff.mag())/brushSize), 0.0f, 1.0f);
                    }
                }
            }
        }

        vec2 centerOfMass = vec2();
        unsigned int numPixels = 0;
        for(unsigned int y = 0; y < 28; ++y)
        {
            for(unsigned int x = 0; x < 28; ++x)
            {
                if(inputActivation[y*28+x] > 0.5f)
                {
                    numPixels++;
                    centerOfMass += vec2(x,y);
                }
            }
        }
        centerOfMass /= (float)numPixels;

        if(GetAsyncKeyState('C')) // center image
        {
            if(!cDown)
            {
                cDown = true;

                int dx = 14-std::round(centerOfMass.x);
                int dy = 14-std::round(centerOfMass.y);

                std::vector<float> tmp;
                tmp.resize(inputActivation.size());

                for(int y = 0; y < 28; ++y)
                {
                    for(int x = 0; x < 28; ++x)
                    {
                        int nx = x-dx;
                        int ny = y-dy;

                        bool oob = false; // out of bounds

                        if(nx < 0) oob=true;
                        if(ny < 0) oob=true;
                        if(nx >= 28) oob=true;
                        if(ny >= 28) oob=true;

                        if(oob) tmp[y*28+x] = 0.0f;
                        else tmp[y*28+x] = inputActivation[ny*28+nx];
                    }
                }

                inputActivation.swap(tmp);
            }
        }
        else cDown = false;

        mainWindow.prepareForRender();

        for(unsigned int y = 0; y < 28; ++y)
        {
            for(unsigned int x = 0; x < 28; ++x)
                shpRender.render(vec2(-1.0f+(float)x*(2.0f/28.0f), 1.0f-(float)y*(2.0f/28.0f)), vec2(1.0f/28.0f,1.0f/28.0f), vec3(inputActivation[y*28+x]), SHP_QUAD);
        }

        shpRender.render(mainWindow.cursorNDC, vec2(1.0f/28.0f,1.0f/28.0f), vec3(1,0,0), SHP_QUAD);
        shpRender.render(vec2(-1.0f+centerOfMass.x*(2.0f/28.0f), 1.0f-centerOfMass.y*(2.0f/28.0f)), vec2(1.0f/28.0f,1.0f/28.0f), vec3(0,0,1), SHP_ELLIPSE);

        Sleep(1); // crude method to clamp frame-rate
    }


    return mainWindow.destroy();
}
