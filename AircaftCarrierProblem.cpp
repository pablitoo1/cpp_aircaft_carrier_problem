#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <algorithm>
#include <random> // dla std::random_device i std::uniform_int_distribution
#include <queue>

using namespace std;

int startingTime = 1501;
int landingTime = 1501;
int actionTime = 1001;
int action_to_stop = 50;
int borderlineNumber = 5;

vector<int> aircraftCarrier;
mutex aircraftMutex;
int actionCount = 0;
queue<int> landingQueue;
queue<int> startingQueue;

sf::RectangleShape landUP(sf::Vector2f(40, 40));
sf::RectangleShape landDOWN(sf::Vector2f(40, 40));

sf::RectangleShape startUP(sf::Vector2f(40, 40));
sf::RectangleShape startDOWN(sf::Vector2f(40, 40));

sf::RectangleShape actionUP(sf::Vector2f(40, 40));
sf::RectangleShape actionDOWN(sf::Vector2f(40, 40));

sf::ConvexShape arrowUp;
sf::ConvexShape arrowDown;

sf::ConvexShape arrowUp2;
sf::ConvexShape arrowDown2;

sf::Texture texture;
sf::Sprite sprite;

sf::Texture texture2;
sf::Sprite sprite2;

void drawState(const queue<int>& landingQueue, const queue<int>& startingQueue, sf::RenderWindow& window) {
    window.clear();

    window.draw(sprite);
    window.draw(sprite2);

    sf::Font font;
    font.loadFromFile("fonts/OpenSans-SemiBold.ttf");

    sf::Text text1;
    text1.setFont(font);
    text1.setCharacterSize(20);
    text1.setFillColor(sf::Color::White);
    text1.setString(L"Ilość dozwolonych akcji:");
    text1.setPosition(30, 785);

    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(24);
    text2.setFillColor(sf::Color::White);
    text2.setString("Samoloty w kolejce do startu:");
    text2.setPosition(30, 650);

    sf::Text text3;
    text3.setFont(font);
    text3.setCharacterSize(24);
    text3.setFillColor(sf::Color::White);
    text3.setString(L"Samoloty w kolejce do lądowania:");
    text3.setPosition(30, 500);

    sf::Text text4;
    text4.setFont(font);
    text4.setCharacterSize(20);
    text4.setFillColor(sf::Color::White);
    text4.setString(L"Ilość wykonanych akcji:");
    text4.setPosition(30, 825);

    sf::Text text5;
    text5.setFont(font);
    text5.setCharacterSize(20);
    text5.setFillColor(sf::Color::White);
    text5.setString(L"Priotytet lądowania jeśli mniej niż:");
    text5.setPosition(30, 865);

    window.draw(text1);
    window.draw(text2);
    window.draw(text3);
    window.draw(text4);
    window.draw(text5);

    sf::Text number1;
    number1.setFont(font);
    number1.setCharacterSize(20);
    number1.setFillColor(sf::Color::Black);

    sf::Text number2;
    number2.setFont(font);
    number2.setCharacterSize(20);
    number2.setFillColor(sf::Color::Black);

    sf::Text number3;
    number3.setFont(font);
    number3.setCharacterSize(20);
    number3.setFillColor(sf::Color::Black);

    sf::Text landUPText;
    landUPText.setFont(font);
    landUPText.setCharacterSize(12);
    landUPText.setFillColor(sf::Color::Black);
    landUPText.setPosition(503, 512);
    landUPText.setString("SLOW");

    sf::Text landDOWNText;
    landDOWNText.setFont(font);
    landDOWNText.setCharacterSize(12);
    landDOWNText.setFillColor(sf::Color::Black);
    landDOWNText.setPosition(556, 512);
    landDOWNText.setString("FAST");

    sf::Text startUPText;
    startUPText.setFont(font);
    startUPText.setCharacterSize(12);
    startUPText.setFillColor(sf::Color::Black);
    startUPText.setPosition(503, 662);
    startUPText.setString("SLOW");

    sf::Text startDOWNText;
    startDOWNText.setFont(font);
    startDOWNText.setCharacterSize(12);
    startDOWNText.setFillColor(sf::Color::Black);
    startDOWNText.setPosition(556, 662);
    startDOWNText.setString("FAST");

    sf::Text actionUPText;
    actionUPText.setFont(font);
    actionUPText.setCharacterSize(12);
    actionUPText.setFillColor(sf::Color::Black);
    actionUPText.setPosition(503, 812);
    actionUPText.setString("SLOW");

    sf::Text actionDOWNText;
    actionDOWNText.setFont(font);
    actionDOWNText.setCharacterSize(12);
    actionDOWNText.setFillColor(sf::Color::Black);
    actionDOWNText.setPosition(556, 812);
    actionDOWNText.setString("FAST");

    landUP.setPosition(500, 500);
    landUP.setFillColor(sf::Color::Red);
    window.draw(landUP);

    landDOWN.setPosition(550, 500);
    landDOWN.setFillColor(sf::Color::Green);
    window.draw(landDOWN);

    startUP.setPosition(500, 650);
    startUP.setFillColor(sf::Color::Red);
    window.draw(startUP);

    startDOWN.setPosition(550, 650);
    startDOWN.setFillColor(sf::Color::Green);
    window.draw(startDOWN);

    actionUP.setPosition(500, 800);
    actionUP.setFillColor(sf::Color::Red);
    window.draw(actionUP);

    actionDOWN.setPosition(550, 800);
    actionDOWN.setFillColor(sf::Color::Green);
    window.draw(actionDOWN);

    sf::Text landValue;
    landValue.setFont(font);
    landValue.setCharacterSize(20);
    landValue.setFillColor(sf::Color::White);
    landValue.setPosition(460, 550);
    landValue.setString("Czas losowania: \n         " + to_string(landingTime) + "ms");
    window.draw(landValue);

    sf::Text startValue;
    startValue.setFont(font);
    startValue.setCharacterSize(20);
    startValue.setFillColor(sf::Color::White);
    startValue.setPosition(460, 700);
    startValue.setString("Czas losowania: \n         " + to_string(startingTime) + "ms");
    window.draw(startValue);

    sf::Text actValue;
    actValue.setFont(font);
    actValue.setCharacterSize(20);
    actValue.setFillColor(sf::Color::White);
    actValue.setPosition(460, 850);
    actValue.setString("Czas losowania: \n         " + to_string(actionTime) + "ms");
    window.draw(actValue);

    sf::Text actionValueToStop;
    actionValueToStop.setFont(font);
    actionValueToStop.setCharacterSize(20);
    actionValueToStop.setFillColor(sf::Color::White);
    actionValueToStop.setPosition(280, 785);
    actionValueToStop.setString(to_string(action_to_stop));
    window.draw(actionValueToStop);

    sf::Text actionValue;
    actionValue.setFont(font);
    actionValue.setCharacterSize(20);
    actionValue.setFillColor(sf::Color::White);
    actionValue.setPosition(280, 825);
    actionValue.setString(to_string(actionCount));
    window.draw(actionValue);

    sf::Text borderlineValue;
    borderlineValue.setFont(font);
    borderlineValue.setCharacterSize(20);
    borderlineValue.setFillColor(sf::Color::White);
    borderlineValue.setPosition(380, 865);
    borderlineValue.setString(to_string(borderlineNumber));
    window.draw(borderlineValue);

    arrowDown.setPointCount(3);
    arrowDown.setPoint(0, sf::Vector2f(320, 802));
    arrowDown.setPoint(1, sf::Vector2f(330, 802));
    arrowDown.setPoint(2, sf::Vector2f(325, 812));
    arrowDown.setFillColor(sf::Color::White);
    window.draw(arrowDown);

    arrowUp.setPointCount(3);
    arrowUp.setPoint(0, sf::Vector2f(320, 797));
    arrowUp.setPoint(1, sf::Vector2f(330, 797));
    arrowUp.setPoint(2, sf::Vector2f(325, 787));
    arrowUp.setFillColor(sf::Color::White);
    window.draw(arrowUp);

    arrowDown2.setPointCount(3);
    arrowDown2.setPoint(0, sf::Vector2f(410, 882));
    arrowDown2.setPoint(1, sf::Vector2f(420, 882));
    arrowDown2.setPoint(2, sf::Vector2f(415, 892));
    arrowDown2.setFillColor(sf::Color::White);
    window.draw(arrowDown2);

    arrowUp2.setPointCount(3);
    arrowUp2.setPoint(0, sf::Vector2f(410, 877));
    arrowUp2.setPoint(1, sf::Vector2f(420, 877));
    arrowUp2.setPoint(2, sf::Vector2f(415, 867));
    arrowUp2.setFillColor(sf::Color::White);
    window.draw(arrowUp2);

    sf::VertexArray line(sf::Lines, 2);

    line[0].position = sf::Vector2f(30, 630);
    line[1].position = sf::Vector2f(600, 630);

    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;

    window.draw(line);

    sf::VertexArray line2(sf::Lines, 2);

    line2[0].position = sf::Vector2f(30, 780);
    line2[1].position = sf::Vector2f(600, 780);

    line2[0].color = sf::Color::White;
    line2[1].color = sf::Color::White;

    window.draw(line2);

    window.draw(landUPText);
    window.draw(landDOWNText);
    window.draw(startUPText);
    window.draw(startDOWNText);
    window.draw(actionUPText);
    window.draw(actionDOWNText);

    int x = 72;
    for (int i = 0; i < aircraftCarrier.size(); i++) {
        sf::Texture planeShape;
        planeShape.loadFromFile("images/plane.png");
        sf::Sprite planeSprite(planeShape);
        if (aircraftCarrier[i] > 9) {
            number1.setPosition(x + 5, 350);
        }
        else {
            number1.setPosition(x + 10, 350);
        }
        number1.setString(to_string(aircraftCarrier[i]));
        planeSprite.setPosition(x, 314);
        window.draw(planeSprite);
        window.draw(number1);
        x += 48;
    }

    x = 30;
    queue<int> copyStartingQueue = startingQueue;
    while (!copyStartingQueue.empty()) {
        sf::RectangleShape planeShape(sf::Vector2f(30, 30));
        if (copyStartingQueue.front() > 9) {
            number2.setPosition(x + 3, 700);
        }
        else {
            number2.setPosition(x + 8, 700);
        }
        number2.setString(to_string(copyStartingQueue.front()));
        planeShape.setPosition(x, 700);
        planeShape.setFillColor(sf::Color::Blue);
        window.draw(planeShape);
        window.draw(number2);
        x += 40;
        copyStartingQueue.pop();
    }

    x = 30;
    queue<int> copyLandingQueue = landingQueue;
    while (!copyLandingQueue.empty()) {
        sf::RectangleShape planeShape(sf::Vector2f(30, 30));
        if (copyLandingQueue.front() > 9) {
            number3.setPosition(x + 3, 550);
        }
        else {
            number3.setPosition(x + 8, 550);
        }
        number3.setString(to_string(copyLandingQueue.front()));
        planeShape.setPosition(x, 550);
        planeShape.setFillColor(sf::Color::Magenta);
        window.draw(planeShape);
        window.draw(number3);
        x += 40;
        copyLandingQueue.pop();
    }

    window.display();
}

int getRandomNumberFromVector(const std::vector<int>& numbers) {
    std::random_device rd;
    std::mt19937 generator(rd());

    if (numbers.empty()) {
        return 0;
    }

    std::uniform_int_distribution<size_t> distribution(0, numbers.size() - 1);

    size_t randomIndex = distribution(generator);

    return numbers[randomIndex];
}


bool shouldExit() {
    if (actionCount >= action_to_stop) {
        return true;
    }
    else {
        return false;
    }
}

void landing() {
    srand(static_cast<unsigned>(time(nullptr)));
    while (!shouldExit()) {
        int plane = -1;
        
        vector <int> notAircraftCarrier;

        notAircraftCarrier.clear();

        for (int i = 1; i <= 20; ++i) {
            if (std::find(aircraftCarrier.begin(), aircraftCarrier.end(), i) == aircraftCarrier.end()) {
                notAircraftCarrier.push_back(i);
            }
        }

        int number = getRandomNumberFromVector(notAircraftCarrier);

        {
            lock_guard<mutex> lock(aircraftMutex);
            queue<int> copyLanding = landingQueue;
            bool inQueue = false;
            while (!copyLanding.empty()) {
                if (copyLanding.front() == number) {
                    inQueue = true;
                    break;
                }
                copyLanding.pop();
            }

            if (aircraftCarrier.size() > 0) {
                if (find(aircraftCarrier.begin(), aircraftCarrier.end(), number) == aircraftCarrier.end() && !inQueue) {
                    plane = number;
                }
            }
            else {
                if (!inQueue) {
                    plane = number;
                }
            }

            if (plane != -1) {
                landingQueue.push(plane);
                cout << "Plane " << plane << " added to landing queue." << endl;
                cout << "Current planes on landing queue: " << landingQueue.size() << " [ ";
                copyLanding = landingQueue;
                while (!copyLanding.empty()) {
                    cout << copyLanding.front() << " ";
                    copyLanding.pop();
                }
                cout << "]" << endl;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1500 + rand() % landingTime));
    }
}

void starting() {
    srand(static_cast<unsigned>(time(nullptr)));
    while (!shouldExit()) {
        int positionPlane = -1;
        queue<int> copyStarting;
        {
            lock_guard<mutex> lock(aircraftMutex);
            if (aircraftCarrier.size() > 0) {
                int plane = getRandomNumberFromVector(aircraftCarrier);
                auto i = find(aircraftCarrier.begin(), aircraftCarrier.end(), plane);

                copyStarting = startingQueue;
                bool inQueue = false;
                while (!copyStarting.empty()) {
                    if (copyStarting.front() == plane) {
                        inQueue = true;
                        break;
                    }
                    copyStarting.pop();
                }

                if (i != aircraftCarrier.end() && !inQueue) {
                    positionPlane = distance(aircraftCarrier.begin(), i);
                }
            }

            if (positionPlane != -1) {
                startingQueue.push(aircraftCarrier[positionPlane]);
                cout << "Plane " << aircraftCarrier[positionPlane] << " added to starting queue." << endl;
                cout << "Current planes on starting queue: " << startingQueue.size() << " [ ";
                copyStarting = startingQueue;
                while (!copyStarting.empty()) {
                    cout << copyStarting.front() << " ";
                    copyStarting.pop();
                }
                cout << "]" << endl;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1500 + rand() % startingTime));
    }
}

void action() {
    srand(static_cast<unsigned>(time(nullptr)));
    while (!shouldExit()) {
        {
            lock_guard<mutex> lock(aircraftMutex);
            if (aircraftCarrier.size() < borderlineNumber) {
                if (!landingQueue.empty()) {
                    int plane = landingQueue.front();
                    if (aircraftCarrier.size() < 10) {
                        landingQueue.pop();
                        aircraftCarrier.push_back(plane);
                        cout << "Plane " << plane << " is landing..." << endl;
                        cout << "\nCurrent planes on carrier: " << aircraftCarrier.size() << " [ ";
                        for (size_t i = 0; i < aircraftCarrier.size(); ++i) {
                            cout << aircraftCarrier[i] << " ";
                        }
                        cout << "]\n" << endl;
                        actionCount++;
                    }
                }
                else if (!startingQueue.empty()) {
                    int planeNumber = startingQueue.front();
                    auto it = find(aircraftCarrier.begin(), aircraftCarrier.end(), planeNumber);
                    if (it != aircraftCarrier.end()) {
                        startingQueue.pop();
                        cout << "Plane " << *it << " is starting..." << endl;
                        aircraftCarrier.erase(it);
                        cout << "\nCurrent planes on carrier: " << aircraftCarrier.size() << " [ ";
                        for (size_t i = 0; i < aircraftCarrier.size(); ++i) {
                            cout << aircraftCarrier[i] << " ";
                        }
                        cout << "]\n" << endl;
                        actionCount++;
                    }
                }
            }
            else {
                if (!startingQueue.empty()) {
                    int planeNumber = startingQueue.front();
                    auto it = find(aircraftCarrier.begin(), aircraftCarrier.end(), planeNumber);
                    if (it != aircraftCarrier.end()) {
                        startingQueue.pop();
                        cout << "Plane " << *it << " is starting..." << endl;
                        aircraftCarrier.erase(it);
                        cout << "\nCurrent planes on carrier: " << aircraftCarrier.size() << " [ ";
                        for (size_t i = 0; i < aircraftCarrier.size(); ++i) {
                            cout << aircraftCarrier[i] << " ";
                        }
                        cout << "]\n" << endl;
                        actionCount++;
                    }
                }
                else if (!landingQueue.empty()) {
                    int plane = landingQueue.front();
                    if (aircraftCarrier.size() < 10) {
                        landingQueue.pop();
                        aircraftCarrier.push_back(plane);
                        cout << "Plane " << plane << " is landing..." << endl;
                        cout << "\nCurrent planes on carrier: " << aircraftCarrier.size() << " [ ";
                        for (size_t i = 0; i < aircraftCarrier.size(); ++i) {
                            cout << aircraftCarrier[i] << " ";
                        }
                        cout << "]\n" << endl;
                        actionCount++;
                    }
                }
            }
        }
        this_thread::sleep_for(chrono::milliseconds(500 + rand() % actionTime));
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(626, 900), "Symulacja Lotniskowca", sf::Style::Titlebar | sf::Style::Close);

    sf::Image icon;
    icon.loadFromFile("images/plane.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    texture.loadFromFile("images/background_water.png");
    sprite.setTexture(texture);

    texture2.loadFromFile("images/bottom.png");
    sprite2.setTexture(texture2);
    sprite2.setPosition(0, 485);
    
    thread landingThread(landing);
    thread startingThread(starting);
    thread actionThread(action);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                if (landUP.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (landingTime < 3500) {
                        landingTime += 500;
                    }
                }
                else if (landDOWN.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (landingTime > 500) {
                        landingTime -= 500;
                    }
                }
                else if (startUP.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (startingTime < 3500) {
                        startingTime += 500;
                    }
                }
                else if (startDOWN.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (startingTime > 500) {
                        startingTime -= 500;
                    }
                }
                else if (actionUP.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (actionTime < 2500) {
                        actionTime += 500;
                    }
                }
                else if (actionDOWN.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (actionTime > 500) {
                        actionTime -= 500;
                    }
                }
                else if (arrowUp.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (action_to_stop < 150 && !shouldExit()) {
                        action_to_stop += 5;
                    }
                }
                else if (arrowDown.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (action_to_stop - 5 >= actionCount) {
                        action_to_stop -= 5;
                    }
                }
                else if (arrowUp2.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (borderlineNumber < 9 && !shouldExit()) {
                        borderlineNumber++;
                    }
                }
                else if (arrowDown2.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (borderlineNumber > 1 && !shouldExit()) {
                        borderlineNumber--;
                    }
                }
            }
        }

        {
            lock_guard<mutex> lock(aircraftMutex);
            drawState(landingQueue, startingQueue, window);
        }

        this_thread::sleep_for(chrono::milliseconds(50));
    }

    landingThread.join();
    startingThread.join();
    actionThread.join();

    return 0;
}