#include <iostream>
#include <iomanip>
#include <conio.h>
#include <limits>
#include <unistd.h>
#include <Windows.h>
#include <SFML/Audio.hpp>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cctype>
#include <iomanip>
#include <mmsystem.h>

using namespace std;
using namespace sf;

const int MAXIMUM_OF_LEVELS = 5;
const int NUMBER_OF_SPECIAL_ITEMS = 2;
const string file1 = "SoloLeaderboard.txt";
const string file2 = "MultiplayerLeaderboard.txt";

struct Player
{
    string name;
    int life;
    int winCount = 0;
    int wins = 0;
    int matches = 0;
    double winPercentage = 0.0;
    int rank;
    int numberOfItems;
    string specialItem[NUMBER_OF_SPECIAL_ITEMS];

    void reset()
    {
        name = "";
        life = 0;
        winCount = 0;
        wins = 0;
        matches = 0;
        numberOfItems = 0;
        for (int i = 0; i < NUMBER_OF_SPECIAL_ITEMS; i++)
            specialItem[i] = "";
    }
};

struct Level
{
    int numberOfBullets;
    int numberOfLiveBullets;
    int numberOfLife;
};

Level levels[MAXIMUM_OF_LEVELS]{
    levels[0] = {3, 1, 1},
    levels[1] = {4, 2, 2},
    levels[2] = {7, 4, 3},
    levels[3] = {8, 5, 3},
    levels[4] = {8, 6, 3}};

struct Node
{
    Player data;
    Node *next;
};

Node *head1 = nullptr;
Node *head2 = nullptr;

struct TreeNode
{
    Player data;
    TreeNode *left, *right;
};

// ---------- ---------- ---------- (EXTRAS) ---------- ---------- ----------
void clear();
void clearInputBuffer();
void pause();
string uppercaseConversion(string name);
void clearSpace(string &name);
bool validateName(string name);
double formatNumber(double number);

// ---------- ---------- - (LEADERBOARD FUNCTIONALITY) - ---------- ----------
// Sort Algorithm
void swap(Node *&node1, Node *&node2);
void heapSwap(Node *node1, Node *node2);
void bubbleSort(Node *&head);
void selectionSort(Node *&head);
void insertionSort(Node *&head);
Node *getTail(Node *cur);
Node *partition(Node *head, Node *end, Node *&newHead, Node *&newEnd);
Node *quickSortRecur(Node *head, Node *end);
void quickSort(Node *&head);
int length(Node *head);
Node *getNodeAtIndex(Node *head, int index);
void heapify(Node *head, int n, int i);
void heapSort(Node *head);
void frontBackSplit(Node *head, Node *&front, Node *&back);
Node *merge(Node *front, Node *back);
void mergeSort(Node *&head);
Node *mergeName(Node *front, Node *back);
void mergeSortByName(Node *&head);
// Search Algorithm
void linearSearch(const string &fileName, Node *head, const string searchName);
Node *middle(Node *start, Node *last);
void binarySearch(const string &fileName, Node *head, const string searchName);
TreeNode *newTreeNode(Player player);
TreeNode *insertTreeNode(TreeNode *root, Player player);
void binarySearchTree(const string &fileName, Node *head, const string searchName);
// Linked List
void insertNode(Node *&head, Player &player);
void displayAllNodes(Node *head);
void getDataFromFile(const string &fileName, Node *&head);
void assignRank(Node *&head);
void clearFileContents(const string &fileName);
void insertDataToFile(const string &fileName, Node *&head);
void displayDataFromFile(const string &fileName, Node *head);

// ---------- ---------- ---------- (MUSIC & SOUNDS) ---------- ---------- ----------
// Music
void stopMusic();
void intro_remix();
void intro();
void music1();
void music2();
void victory();
void gameover();
// Sounds
void warning();
void exitSound();
void thud();
void rack();
void reload();
void gunclick();
void gunshot();
void death();
// Dialogue
void dialogue1();
void dialogue2();
void dialogue3();

// ---------- ---------- ---------- (VISUALS, TEXT, IMAGES) ---------- ---------- ----------
void loading();
void titleStatic();
void title();
void titleArt();
void imageGameover();
void imageVictory();
void imageBotAngry();
void imageBot();
void imagePlayer2();
void imagePlayer1();
void imageGunReloaded();
void imageGunReload();
void imageGunHold();
void imageGunShot();
void imageGunShoot();
void imageGunPoint();
void imageGunShotSelf();
void imageGunShootSelf();
void imageGunPointSelf();
void imageGunShootPlayer();

// ---------- ---------- ---------- (ANIMATIONS) ---------- ---------- ----------
// ANIMATION: Dialogues
void playDialogue1();
void playDialogue2();
void playDialogue3();

// ANIMAZTION: End Game (Victory & Gameover)
void victoryScreen();
void gameoverScreen();

// ANIMATION: Holding Gun
void armedBot();
void armedPlayer();
void armedPlayer2();

// ANIMATION: SOLO GAMEPLAY
void reloadBot(Level level);
void shootBot();
void shootBotBlank();
void shootPlayer();
void shootPlayerBlank();
void playerShootsSelf();
void playerShootsSelfBlank();
void botShootsSelf();
void botShootsSelfBlank();

// ANIMATION - MULTIPLAYER GAMEPLAY
void player1ShootsPlayer2();
void player1ShootsPlayer2Blank();
void player1ShootsSelf();
void player1ShootsSelfBlank();
void player2ShootsPlayer1();
void player2ShootsPlayer1Blank();
void player2ShootsSelf();
void player2ShootsSelfBlank();

// ---------- ---------- ---------- (ALL MENUS GUI) ---------- ---------- ----------
void leaderboard();
void option();
void optionWithSpecial();
void specialItemOption();
void searchOption();
void searchAlgorithmOption();
void searchLeaderboard(const string &file, Node *&head);
void displayBullets(Level level);
void displayLife(Player &player1, Player &player2);
void specialItemOption();
void credits();
void menu();
void start();
void titleScreen();

// ---------- ---------- ----- (IN-GAME MECHANICS) ----- ---------- ----------
int botChoice(Player &bot, int numberOfOptions);
void randomizeSpecialItem(Player &player);
void displaySpecialItem(Player &player);
void deleteSpecialItem(Player &player, int select);
void specialItemOperation(Player &player, int specialItemIndex, string bullets[], int currentBulletIndex, bool &inspectStatus);
void displayTurn(Player &player1, Player &player2, bool solo, bool player1Turn, const string &currentPlayerName);
void gameLevel(Player &player1, Player &player2, Level level, bool player1Turn);
void specialLevel(Player &player1, Player &player2, Level level, bool player1Turn);
void gamePlay(Player &player1, Player &player2, bool player1Turn);
void playTurn(Player &player1, Player &player2);
void soloMode(Player &player, Player &bot, const string &file);
void multiplayerMode(Player &player1, Player &player2, const string &file);

int main()
{
    getDataFromFile(file1, head1);
    getDataFromFile(file2, head2);

    srand(time(0));

    clear();
    titleScreen();
    start();

    return 0;
}

// ---------- ---------- ---------- (EXTRAS) ---------- ---------- ----------
void clear()
{
#ifdef _WIN32 // For Windows
    system("cls");
#else // For Unix-based systems (like Linux)
    system("clear");
#endif
}

void clearInputBuffer()
{
    cin.clear();                                         // Clear any error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the input buffer
}

void pause()
{
    cout << "\n\t\t\t\t             PRESS ENTER TO CONTINUE...";
    cin.get();
}

string uppercaseConversion(string name)
{
    for (int i = 0; i < name.length(); i++)
        name[i] = toupper(name[i]);
    return name;
}

void clearSpace(string &name)
{
    int spaceIndex = -1;
    int length = name.length();
    for (int i = 0; i < name.length(); i++)
    {
        if (name[i] != ' ')
            break;
        spaceIndex = i;
        length--;
    }
    name = name.substr(spaceIndex + 1, length);
}

bool validateName(string name)
{
    clearSpace(name);
    name = uppercaseConversion(name);
    for (int i = 0; i < name.length(); i++)
        if (!isalpha(name[i]))
            return false;

    if (name.empty() || name.length() > 8 || name == "DEALER")
        return false;
    return true;
}

double formatNumber(double number)
{
    return static_cast<int>(number * 100) / 100.0;
}

// ---------- ---------- - (LEADERBOARD FUNCTIONALITY) - ---------- ----------
// Sort Algorithm
void swap(Node *&node1, Node *&node2)
{
    Player temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

void heapSwap(Node *node1, Node *node2)
{
    Player temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

void bubbleSort(Node *&head)
{
    if (head == nullptr || head->next == nullptr)
        return;

    bool swapped;
    Node *temp;
    Node *ltemp = nullptr;

    do
    {
        swapped = false;
        temp = head;

        while (temp->next != ltemp)
        {
            if ((temp->next->data.winPercentage > temp->data.winPercentage) || (temp->next->data.winPercentage == temp->data.winPercentage && temp->next->data.matches > temp->data.matches))
            {
                swap(temp, temp->next);
                swapped = true;
            }
            temp = temp->next;
        }
        ltemp = temp;
    } while (swapped);
}

void selectionSort(Node *&head)
{
    Node *temp1 = head;
    while (temp1 != nullptr)
    {
        Node *max = temp1;
        Node *temp2 = temp1->next;

        while (temp2 != nullptr)
        {
            if ((temp2->data.winPercentage > max->data.winPercentage) || (temp2->data.winPercentage == max->data.winPercentage && temp2->data.matches > max->data.matches))
                max = temp2;

            temp2 = temp2->next;
        }

        swap(temp1, max);
        temp1 = temp1->next;
    }
}

void insertionSort(Node *&head)
{
    if (head == nullptr || head->next == nullptr)
        return;

    Node *sorted = nullptr;
    Node *temp = head;

    while (temp != nullptr)
    {
        Node *nextNode = temp->next;
        if (sorted == nullptr || (temp->data.winPercentage > sorted->data.winPercentage) || (temp->data.winPercentage == sorted->data.winPercentage && temp->data.matches > sorted->data.matches))
        {
            temp->next = sorted;
            sorted = temp;
        }
        else
        {
            Node *traverse = sorted;
            while (traverse->next != nullptr && ((traverse->next->data.winPercentage > temp->data.winPercentage) || (traverse->next->data.winPercentage == temp->data.winPercentage && traverse->next->data.matches > temp->data.matches)))
            {
                traverse = traverse->next;
            }
            temp->next = traverse->next;
            traverse->next = temp;
        }
        temp = nextNode;
    }

    head = sorted;
}

Node *getTail(Node *cur)
{
    while (cur != nullptr && cur->next != nullptr)
        cur = cur->next;
    return cur;
}

Node *partition(Node *head, Node *end, Node *&newHead, Node *&newEnd)
{
    Node *pivot = end;
    Node *prev = nullptr, *cur = head, *tail = pivot;

    while (cur != pivot)
    {
        if ((cur->data.winPercentage > pivot->data.winPercentage) || (cur->data.winPercentage == pivot->data.winPercentage && cur->data.matches > pivot->data.matches))
        {
            if (newHead == nullptr)
                newHead = cur;

            prev = cur;
            cur = cur->next;
        }
        else
        {
            if (prev)
                prev->next = cur->next;

            Node *tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if (newHead == nullptr)
        newHead = pivot;

    newEnd = tail;

    return pivot;
}

Node *quickSortRecur(Node *head, Node *end)
{
    if (head == nullptr || head == end)
        return head;

    Node *newHead = nullptr, *newEnd = nullptr;

    Node *pivot = partition(head, end, newHead, newEnd);

    if (newHead != pivot)
    {
        Node *tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = nullptr;

        newHead = quickSortRecur(newHead, tmp);

        tmp = getTail(newHead);
        tmp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd);

    return newHead;
}

void quickSort(Node *&head)
{
    head = quickSortRecur(head, getTail(head));
    return;
}

int length(Node *head)
{
    int len = 0;
    Node *current = head;
    while (current != nullptr)
    {
        len++;
        current = current->next;
    }
    return len;
}

Node *getNodeAtIndex(Node *head, int index)
{
    Node *current = head;
    for (int i = 0; i < index && current != nullptr; i++)
        current = current->next;
    return current;
}

void heapify(Node *head, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && ((getNodeAtIndex(head, left)->data.winPercentage < getNodeAtIndex(head, largest)->data.winPercentage) || (getNodeAtIndex(head, left)->data.winPercentage == getNodeAtIndex(head, largest)->data.winPercentage && getNodeAtIndex(head, left)->data.matches < getNodeAtIndex(head, largest)->data.matches)))
        largest = left;

    if (right < n && ((getNodeAtIndex(head, right)->data.winPercentage < getNodeAtIndex(head, largest)->data.winPercentage) || (getNodeAtIndex(head, right)->data.winPercentage == getNodeAtIndex(head, largest)->data.winPercentage && getNodeAtIndex(head, right)->data.matches < getNodeAtIndex(head, largest)->data.matches)))
        largest = right;

    if (largest != i)
    {
        heapSwap(getNodeAtIndex(head, i), getNodeAtIndex(head, largest));
        heapify(head, n, largest);
    }
}

void heapSort(Node *head)
{
    int n = length(head);

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(head, n, i);

    for (int i = n - 1; i > 0; i--)
    {
        heapSwap(getNodeAtIndex(head, 0), getNodeAtIndex(head, i));
        heapify(head, i, 0);
    }
}

void frontBackSplit(Node *head, Node *&front, Node *&back)
{
    Node *slow = head;
    Node *fast = head->next;

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    front = head;
    back = slow->next;
    slow->next = nullptr;
}

Node *merge(Node *front, Node *back)
{
    Node *temp = nullptr;

    if (front == nullptr)
        return back;
    else if (back == nullptr)
        return front;

    if ((front->data.winPercentage > back->data.winPercentage) || (front->data.winPercentage == back->data.winPercentage && front->data.matches > back->data.matches))
    {
        temp = front;
        temp->next = merge(front->next, back);
    }
    else
    {
        temp = back;
        temp->next = merge(front, back->next);
    }

    return temp;
}

void mergeSort(Node *&head)
{
    Node *temp = head;
    Node *front;
    Node *back;

    if (temp == nullptr || temp->next == nullptr)
        return;

    frontBackSplit(temp, front, back);

    mergeSort(front);
    mergeSort(back);

    head = merge(front, back);
}

Node *mergeName(Node *front, Node *back)
{
    Node *temp = nullptr;

    if (front == nullptr)
        return back;
    else if (back == nullptr)
        return front;

    if (front->data.name < back->data.name)
    {
        temp = front;
        temp->next = mergeName(front->next, back);
    }
    else
    {
        temp = back;
        temp->next = mergeName(front, back->next);
    }

    return temp;
}

void mergeSortByName(Node *&head)
{
    Node *temp = head;
    Node *front;
    Node *back;

    if (temp == nullptr || temp->next == nullptr)
        return;

    frontBackSplit(temp, front, back);

    mergeSortByName(front);
    mergeSortByName(back);

    head = mergeName(front, back);
}

// Search Algorithm
void linearSearch(const string &fileName, Node *head, const string searchName)
{
    Node *temp = head;
    bool found = false;
    cout << "\n";
    cout << "\t\t\t\t\t     " << ((fileName == file1) ? "    --- SOLO LEADERBOARD ---   \n" : "--- MULTIPLAYER LEADERBOARD ---\n")
         << "\t\t  -----------------------------------------------------------------------------------\n"
         << "\t\t  " << left << setw(20) << "Player Name" << right << setw(20) << "Matches Played" << setw(12) << "Wins" << setw(17) << "Wins(%)" << setw(15) << "Rank\n"
         << "\t\t  -----------------------------------------------------------------------------------\n";
    while (temp != nullptr)
    {
        if (temp->data.name.find(searchName) != string::npos)
        {
            cout << "\t\t  " << left << setw(20) << temp->data.name << right << setw(20) << temp->data.matches << setw(12) << temp->data.wins << setw(17) << temp->data.winPercentage << setw(14) << temp->data.rank << endl;
            usleep(50000);
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
        cout << "\t\t                                    NO DATA FOUND\n";

    cout << "\t\t  -----------------------------------------------------------------------------------\n";
}

Node *middle(Node *start, Node *last)
{
    if (start == nullptr)
        return nullptr;

    Node *slow = start;
    Node *fast = start->next;

    while (fast != last)
    {
        fast = fast->next;
        if (fast != last)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    return slow;
}

void binarySearch(const string &fileName, Node *head, const string searchName)
{
    mergeSortByName(head);
    bool found = false;
    Node *start = head;
    Node *last = nullptr;
    cout << "\n";
    cout << "\t\t\t\t\t     " << ((fileName == file1) ? "    --- SOLO LEADERBOARD ---   \n" : "--- MULTIPLAYER LEADERBOARD ---\n")
         << "\t\t  -----------------------------------------------------------------------------------\n"
         << "\t\t  " << left << setw(20) << "Player Name" << right << setw(20) << "Matches Played" << setw(12) << "Wins" << setw(17) << "Wins(%)" << setw(15) << "Rank\n"
         << "\t\t  -----------------------------------------------------------------------------------\n";

    do
    {
        Node *mid = middle(start, last);

        if (mid == nullptr)
            break;

        if (mid->data.name == searchName)
        {
            cout << "\t\t  " << left << setw(20) << mid->data.name << right << setw(20) << mid->data.matches << setw(12) << mid->data.wins << setw(17) << mid->data.winPercentage << setw(14) << mid->data.rank << endl;
            usleep(40000);
            found = true;
            break;
        }
        else if (mid->data.name < searchName)
            start = mid->next;
        else
            last = mid;

    } while (last == nullptr || last != start);

    if (!found)
        cout << "\t\t                                    NO DATA FOUND\n";

    cout << "\t\t  -----------------------------------------------------------------------------------\n";

    mergeSort(head);
}

TreeNode *newTreeNode(Player player)
{
    TreeNode *temp = new TreeNode;
    temp->data = player;
    temp->left = temp->right = nullptr;
    return temp;
}

TreeNode *insertTreeNode(TreeNode *root, Player player)
{
    if (root == NULL)
        return newTreeNode(player);

    if (player.name < root->data.name)
        root->left = insertTreeNode(root->left, player);
    else if (player.name > root->data.name)
        root->right = insertTreeNode(root->right, player);

    return root;
}

void binarySearchTree(const string &fileName, Node *head, const string searchName)
{
    Node *temp = head;
    bool found = false;
    TreeNode *root = nullptr;

    cout << "\n";
    cout << "\t\t\t\t\t     " << ((fileName == file1) ? "    --- SOLO LEADERBOARD ---   \n" : "--- MULTIPLAYER LEADERBOARD ---\n")
         << "\t\t  -----------------------------------------------------------------------------------\n"
         << "\t\t  " << left << setw(20) << "Player Name" << right << setw(20) << "Matches Played" << setw(12) << "Wins" << setw(17) << "Wins(%)" << setw(15) << "Rank\n"
         << "\t\t  -----------------------------------------------------------------------------------\n";

    root = insertTreeNode(root, temp->data);
    while (temp->next != nullptr)
    {
        insertTreeNode(root, temp->next->data);
        temp = temp->next;
    }

    while (root != nullptr)
    {
        if (searchName > root->data.name)
            root = root->right;
        else if (searchName < root->data.name)
            root = root->left;
        else
        {
            cout << "\t\t  " << left << setw(20) << root->data.name << right << setw(20) << root->data.matches << setw(12) << root->data.wins << setw(17) << root->data.winPercentage << setw(14) << root->data.rank << endl;
            usleep(40000);
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\t\t                                    NO DATA FOUND\n";

    cout << "\t\t  -----------------------------------------------------------------------------------\n";
}

// Linked List
void insertNode(Node *&head, Player &player)
{
    Node *temp = head;

    // Check if the node with the same name already exists
    while (temp != nullptr)
    {
        if (temp->data.name == player.name)
        {
            temp->data.wins += player.wins;
            temp->data.matches += player.matches;
            temp->data.winPercentage = formatNumber(temp->data.wins * 100.0 / temp->data.matches);
            return;
        }
        temp = temp->next;
    }

    Node *newNode = new Node;
    newNode->data = player;
    newNode->next = nullptr;

    if (head == nullptr)
        head = newNode;

    else
    {
        temp = head;
        while (temp->next != nullptr)
            temp = temp->next;

        temp->next = newNode;
    }
}

void displayAllNodes(Node *head)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        usleep(50000);
        cout << "\t\t  " << left << setw(20) << temp->data.name << right << setw(20) << temp->data.matches << setw(12) << temp->data.wins << setw(17) << temp->data.winPercentage << setw(14) << temp->data.rank << endl;
        temp = temp->next;
    }
}

void getDataFromFile(const string &fileName, Node *&head)
{
    ifstream file(fileName);

    string line;
    int lineCount = 0;
    while (getline(file, line))
    {
        lineCount++;
        if (lineCount < 5)
            continue;

        if (!file.eof())
        {
            Player player;
            file >> player.name >> player.matches >> player.wins >> player.winPercentage >> player.rank;
            insertNode(head, player);
        }
    }

    file.close();
}

void assignRank(Node *&head)
{
    Node *temp = head;
    int rank = 1;

    while (temp != nullptr)
    {
        temp->data.rank = rank;
        if (temp->next != nullptr && (temp->data.winPercentage != temp->next->data.winPercentage || temp->data.matches != temp->next->data.matches))
            rank++;
        temp = temp->next;
    }
}

void clearFileContents(const string &fileName)
{
    ofstream file(fileName, ios::trunc);

    file << "\t\t\t\t\t     " << ((fileName == file1) ? "    --- SOLO LEADERBOARD ---   \n" : "--- MULTIPLAYER LEADERBOARD ---\n")
         << "\t\t  -----------------------------------------------------------------------------------\n"
         << "\t\t  " << left << setw(20) << "Player Name" << right << setw(20) << "Matches Played" << setw(12) << "Wins" << setw(17) << "Wins(%)" << setw(15) << "Rank\n"
         << "\t\t  -----------------------------------------------------------------------------------\n";
    file.close();
}

void insertDataToFile(const string &fileName, Node *&head)
{
    clearFileContents(fileName);

    ofstream file(fileName, ios::app);

    mergeSort(head);
    assignRank(head);

    Node *temp = head;
    while (temp != nullptr)
    {
        file << "\n\t\t  "
             << left << setw(20) << temp->data.name
             << right << setw(20) << temp->data.matches
             << setw(17) << temp->data.wins
             << setw(12) << temp->data.winPercentage
             << setw(14) << temp->data.rank;
        temp = temp->next;
    }

    file.close();
}

void displayDataFromFile(const string &fileName, Node *head)
{
    // rack();
    clear();
    titleStatic();
    cout << "\n";
    cout << "\t\t\t\t\t     " << ((fileName == file1) ? "    --- SOLO LEADERBOARD ---   \n" : "--- MULTIPLAYER LEADERBOARD ---\n")
         << "\t\t  -----------------------------------------------------------------------------------\n"
         << "\t\t  " << left << setw(20) << "Player Name" << right << setw(20) << "Matches Played" << setw(12) << "Wins" << setw(17) << "Wins(%)" << setw(15) << "Rank\n"
         << "\t\t  -----------------------------------------------------------------------------------\n";

    string sorts[] = {"BUBBLE", "INSERTION", "SELECTION", "MERGE", "QUICK", "HEAP"};
    int r = rand() % 6;
    string s = "SORTED BY " + sorts[r] + " SORT ALGORITHM";
    switch (r)
    {
    case 0:
        bubbleSort(head);
        break;
    case 1:
        insertionSort(head);
        break;
    case 2:
        selectionSort(head);
        break;
    case 3:
        mergeSort(head);
        break;
    case 4:
        quickSort(head);
        break;
    case 5:
        heapSort(head);
        break;
    }

    displayAllNodes(head);

    cout << "\t\t  -----------------------------------------------------------------------------------\n";

    int dif = 35 - s.length();
    for (int i = 0; i < dif; i++)
        s = ' ' + s;

    cout << left << setw(67);
    for (int i = 0; i < s.length(); i++)
    {
        cout << s[i];
        Sleep(30);
    }
}

// ---------- ---------- ---------- (MUSIC & SOUNDS) ---------- ---------- ----------
// Music
void stopMusic() // Only works with other PlaySound()
{
    PlaySound(NULL, 0, SND_PURGE);
}
void intro_remix()
{
    PlaySound(TEXT("audio/music/intro_remix.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void intro()
{
    PlaySound(TEXT("audio/music/intro_low.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void music1()
{
    PlaySound(TEXT("audio/music/music1_low.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void music2()
{
    PlaySound(TEXT("audio/music/music2_low.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void victory()
{
    PlaySound(TEXT("audio/music/victory.wav"), NULL, SND_ASYNC | SND_LOOP);
}
void gameover()
{
    PlaySound(TEXT("audio/music/gameover.wav"), NULL, SND_ASYNC);
}
// Sounds
void exitSound()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/fart.wav");

    Sound reload;
    reload.setVolume(100);
    reload.setBuffer(buffer);
    reload.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void warning()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/warning.wav");

    Sound warning;
    warning.setVolume(100);
    warning.setBuffer(buffer);
    warning.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void thud()
{
    PlaySound(TEXT("audio/sound/thud.wav"), NULL, SND_ASYNC);
}

void rack()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/rack1.wav");

    Sound rack;
    rack.setBuffer(buffer);
    rack.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void reload()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/reload.wav");

    Sound reload;
    reload.setVolume(75);
    reload.setBuffer(buffer);
    reload.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void gunclick()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/gunclick.wav");

    Sound gunclick;
    gunclick.setVolume(20);
    gunclick.setBuffer(buffer);
    gunclick.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void gunshot()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/gunshot.wav");

    Sound gunshot;
    gunshot.setVolume(100);
    gunshot.setBuffer(buffer);
    gunshot.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void death()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/sound/death.wav");

    Sound death;
    death.setVolume(100);
    death.setBuffer(buffer);
    death.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}
// Dialogue
void dialogue1()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/dialogue/dialogue1.wav");

    Sound dialogue1;
    dialogue1.setVolume(25);
    dialogue1.setBuffer(buffer);
    dialogue1.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void dialogue2()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/dialogue/dialogue2.wav");

    Sound dialogue2;
    dialogue2.setVolume(25);
    dialogue2.setBuffer(buffer);
    dialogue2.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

void dialogue3()
{
    SoundBuffer buffer;
    buffer.loadFromFile("audio/dialogue/dialogue3.wav");

    Sound dialogue3;
    dialogue3.setVolume(25);
    dialogue3.setBuffer(buffer);
    dialogue3.play();

    Sleep(buffer.getDuration().asMilliseconds()); // Pauses everything until sound is done playing
}

// ---------- ---------- ---------- (VISUALS, TEXT, IMAGES) ---------- ---------- ----------
void loading()
{
    for (int i = 0; i < 2; i++)
    {
        clear();
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t   [ LOADING GAME ]   \n";
        usleep(400500);
        clear();
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  -[ LOADING GAME ]-  \n";
        usleep(15000);
        clear();
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t - [ LOADING GAME ] - \n";
        usleep(15000);
        clear();
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t-  [ LOADING GAME ]  -\n";
        usleep(15000);
        clear();
    }
}
void titleStatic()
{
    //	cout << "\n";
    cout << "\t\t           _________ ___ ___ ___________________________ ____ __________                      \n";
    cout << "\t\t          /   _____//   |   \\\\_____  \\__    ___/  _____/|    |   \\      \\                \n";
    cout << "\t\t          \\_____  \\/    ~    \\/   |   \\|    | /   \\  ___|    |   /   |   \\              \n";
    cout << "\t\t          /        \\    Y    /    |    \\    | \\    \\_\\  \\    |  /    |    \\            \n";
    cout << "\t\t         /_______  /\\___|_  /\\_______  /____|  \\______  /______/\\____|__  /               \n";
    cout << "\t\t __________ _____\\/_  ____\\/_________\\/  _____________\\/________________\\/___________    \n";
    cout << "\t\t \\______   \\\\_____  \\ |    |   \\    |    \\_   _____/\\__    ___/\\__    ___/\\_   _____/\n";
    cout << "\t\t  |       _/ /   |   \\|    |   /    |     |    __)_   |    |     |    |    |    __)_         \n";
    cout << "\t\t  |    |   \\/    |    \\    |  /|    |___  |        \\  |    |     |    |    |        \\     \n";
    cout << "\t\t  |____|_  /\\_______  /______/ |_______ \\/_______  /  |____|     |____|   /_______  /       \n";
    cout << "\t\t         \\/         \\/                 \\/        \\/                               \\/     \n";
}
void title()
{
    //	cout << "\n";
    cout << "\t\t           _________ ___ ___ ___________________________ ____ __________                      \n";
    usleep(50000);
    cout << "\t\t          /   _____//   |   \\\\_____  \\__    ___/  _____/|    |   \\      \\                \n";
    usleep(50000);
    cout << "\t\t          \\_____  \\/    ~    \\/   |   \\|    | /   \\  ___|    |   /   |   \\              \n";
    usleep(50000);
    cout << "\t\t          /        \\    Y    /    |    \\    | \\    \\_\\  \\    |  /    |    \\            \n";
    usleep(50000);
    cout << "\t\t         /_______  /\\___|_  /\\_______  /____|  \\______  /______/\\____|__  /               \n";
    usleep(50000);
    cout << "\t\t __________ _____\\/_  ____\\/_________\\/  _____________\\/________________\\/___________    \n";
    usleep(50000);
    cout << "\t\t \\______   \\\\_____  \\ |    |   \\    |    \\_   _____/\\__    ___/\\__    ___/\\_   _____/\n";
    usleep(50000);
    cout << "\t\t  |       _/ /   |   \\|    |   /    |     |    __)_   |    |     |    |    |    __)_         \n";
    usleep(50000);
    cout << "\t\t  |    |   \\/    |    \\    |  /|    |___  |        \\  |    |     |    |    |        \\     \n";
    usleep(50000);
    cout << "\t\t  |____|_  /\\_______  /______/ |_______ \\/_______  /  |____|     |____|   /_______  /       \n";
    usleep(50000);
    cout << "\t\t         \\/         \\/                 \\/        \\/                               \\/     \n";
    usleep(310000);
}
void titleArt()
{
    cout << "\t\tOriginal Game By                   .,,,,,,,,..                          Fan Game By\n"
            "\t\t Mike  Klubnika             ..,,/***/**,//,,.///,,/,..               PIU CS Department\n"
            "\t\t\t\t          .,,,,,*/%,(#(/*,,,.,( ,,(//,,*..          Year 2  (2023-2024)\n"
            "\t\t\t\t       .,,.,..     ...,.,.....  ....,*/***,.     \n"
            "\t\t\t\t     ....,.         ..,,........        ,,,,,.   \n"
            "\t\t\t\t    .,*...           ,,,**,,...         .,*,,,.  \n"
            "\t\t\t\t   ...,*..           .%%%%%*....        ..**.... \n"
            "\t\t\t\t   *#(*,%(%(/,,**#(/*%%%@&%%#,,,,.....   .,,,,.. \n"
            "\t\t\t\t  .,      .(%%&@@@%@%%%%@@&%,,,,,,,,,*,,.  ..,,. \n"
            "\t\t\t\t  .,      ,...,(%&@&%(#/*&#/.,*,*%*/..        .. \n"
            "\t\t\t\t  .,    ., ...   .*,,*//*/,**,,.. .....       .  \n"
            "\t\t\t\t   .,.   .  .. .         ...       ..   ,        \n"
            "\t\t\t\t    .,,.    .   . . ....        .   ..           \n"
            "\t\t\t\t      ...,..     .  .  ..                        \n"
            "\t\t\t\t        ....,,...                                \n"
            "\t\t\t\t           . . ..........                        \n";
}
void imageGameover()
{
    cout << "\n\n\n\n\n\n\n\n\n";
    cout << "\t\t\t\t   ______                        ____                    \n";
    usleep(100000);
    cout << "\t\t\t\t  / ____/___ _____ ___  ___     / __ \\_   _____  _____  \n";
    usleep(100000);
    cout << "\t\t\t\t / / __/ __ `/ __ `__ \\/ _ \\   / / / / | / / _ \\/ ___/\n";
    usleep(100000);
    cout << "\t\t\t\t/ /_/ / /_/ / / / / / /  __/  / /_/ /| |/ /  __/ /       \n";
    usleep(100000);
    cout << "\t\t\t\t\\____/\\__,_/_/ /_/ /_/\\___/   \\____/ |___/\\___/_/   \n";
    usleep(100000);
}
void imageVictory()
{
    cout << "\n\n\n\n\n\n\n\n\n";
    cout << "\t\t\t\t     _____                  _                ____   \n";
    usleep(100000);
    cout << "\t\t\t\t    / ___/__  ________   __(_)   _____  ____/ / /   \n";
    usleep(100000);
    cout << "\t\t\t\t    \\__ \\/ / / / ___/ | / / / | / / _ \\/ __  / / \n";
    usleep(100000);
    cout << "\t\t\t\t   ___/ / /_/ / /   | |/ / /| |/ /  __/ /_/ /_/     \n";
    usleep(100000);
    cout << "\t\t\t\t  /____/\\__,_/_/    |___/_/ |___/\\___/\\__,_(_)   \n";
    usleep(100000);
}
void imageBotAngry()
{
    cout << "\t\t\t\t                  ..,,,,,,,,,..                  \n"
            "\t\t\t\t           ...,//***/**,/(//,.//////,..          \n"
            "\t\t\t\t         ..,,.,,...,. . .,, ,.,. ,,..,,,*..      \n"
            "\t\t\t\t       .,,,,..       . . , . ... .. .,*/***,.    \n"
            "\t\t\t\t     ..,.,.        ...,.,..........    .....,.   \n"
            "\t\t\t\t    .,*...          ..,,.......         ....*,,, \n"
            "\t\t\t\t   ...,*..           ,,,***,,..         ...**... \n"
            "\t\t\t\t   *#(*#%%%,.        .%%%@%%*..       . *.*,,,. .\n"
            "\t\t\t\t  .%(%##@@%%%%&**#(/*%%%@@&%%#,,,,...*@*.#.*,.,, \n"
            "\t\t\t\t  ...   .,(%%%%%%%%@%%%%@@%#%%%/,*.*,%%%%%%@,,,..\n"
            "\t\t\t\t  .,.     %#%%(@@@%@%%%%@@@&%,,,,,,,,#%#(#/*.... \n"
            "\t\t\t\t  .,..  ., ... (%&@&%(#/*#&#/.,*,*%*/  .*//*   . \n"
            "\t\t\t\t  ..,.   .   .. .          ...       ..   ,   .  \n"
            "\t\t\t\t   ..,,.    .   . . ....        .   ..      .    \n"
            "\t\t\t\t     ....    .    .  .  ..                       \n"
            "\t\t\t\t       .....,,... .         .                    \n"
            "\t\t\t\t          .. . ..........   . .                  \n";
}
void imageBot()
{
    cout << "\t\t\t\t                   .,,,,,,,,,..                  \n"
            "\t\t\t\t            ..,,/***/**,/(/,,.///,,/,..          \n"
            "\t\t\t\t          .,,,,,*/%,(#(/*,,,,.,( ,,(//,,*..      \n"
            "\t\t\t\t       .,,.,..     ...,.,......  ....,*/***,.    \n"
            "\t\t\t\t     ....,.         ..,,.........        ,,,,,.  \n"
            "\t\t\t\t    .,*...           ,,,***,,...         .,*,,,. \n"
            "\t\t\t\t   ...,*..           .%%%@%%*....        ..**....\n"
            "\t\t\t\t   *#(*,%(%(/,,**#(/*%%%@@&%%#,,,,.....   .,,,,..\n"
            "\t\t\t\t  ..    .,(%%%%%%%%@%%%%@@%#%%%/,*.*,/**,,,,,,,..\n"
            "\t\t\t\t  .,      .(%%&@@@%@%%%%@@@&%,,,,,,,,,*,,.  ..,,.\n"
            "\t\t\t\t  .,      ,...,(%&@&%(#/*#&#/.,*,*%*/..        ..\n"
            "\t\t\t\t  .,    ., ...   .*,,*//**/,**,,.. .....       . \n"
            "\t\t\t\t   .,.   .  .. .          ...       ..   ,       \n"
            "\t\t\t\t    .,,.    .   . . ....         .   ..          \n"
            "\t\t\t\t      ...,..     .  .  ..                        \n"
            "\t\t\t\t        ....,,...                                \n"
            "\t\t\t\t           . . ..........                        \n";
}
void imagePlayer2()
{
    cout << "\t\t\t\t              ,((((((((#((((((((**,             \n"
            "\t\t\t\t            (###########(*******((*,            \n"
            "\t\t\t\t           .###%%%#####(***********             \n"
            "\t\t\t\t           (###%%####/,,*//,....***             \n"
            "\t\t\t\t           ###%/(  **             .             \n"
            "\t\t\t\t           ###%//  ...             .            \n"
            "\t\t\t\t          .###%//    ..            ..           \n"
            "\t\t\t\t         . . (,##                 . .           \n"
            "\t\t\t\t        (  %#%###/          ..     . .          \n"
            "\t\t\t\t       * ( /(##%%           ...    . .          \n"
            "\t\t\t\t       / ,, /##(#                  ...          \n"
            "\t\t\t\t       (.,%%##%/(                (((//*         \n"
            "\t\t\t\t       ../&(///(#/*,/**.        .*///...        \n"
            "\t\t\t\t      (/((/(//////******,,...,*/#%%%.//*(((/*   \n"
            "\t\t\t\t   ,%(,,/(//((/********,,,,,,**//(((,,,(%####(/ \n"
            "\t\t\t\t  %%%#.  ,/(//*********,,,,,,,,***     .    ..,,\n"
            "\t\t\t\t %%%#/,    ..,**.***,,,,,,,,,,,,,          .....\n";
}
void imagePlayer1()
{
    cout << "\t\t\t\t                   .*////*****,.                \n"
            "\t\t\t\t               ./(#%%%%%%%#((///#(,             \n"
            "\t\t\t\t              .,,*/(##(//***/(###%%(            \n"
            "\t\t\t\t              .....,.......,*(###%%%            \n"
            "\t\t\t\t             ..             ..////*%*           \n"
            "\t\t\t\t             .                 //(*%%           \n"
            "\t\t\t\t             . .                ///%*%%         \n"
            "\t\t\t\t             ..               /####(###,        \n"
            "\t\t\t\t            ..    ...      ..,*/ (/####%.       \n"
            "\t\t\t\t            ..   ..    . ..,*/   #%##(/         \n"
            "\t\t\t\t             .         .....,   .#######,.      \n"
            "\t\t\t\t            #((                  #(##%%&(,      \n"
            "\t\t\t\t        .####/*, .          ,/,,*//////(.       \n"
            "\t\t\t\t    */((###*%%%#(/,,...,,*,******//////(((/,.,  \n"
            "\t\t\t\t  /(#%%%%,,,.#((/***,,,,,,,,*******//(///*.../  \n"
            "\t\t\t\t *,,.   ..    ***,,,,,,,,,,,,****,.....*   .##% \n"
            "\t\t\t\t....*.           .,,,,,,,,,,,,,,,*,.     * .*,%%\n";
}
void imageGunReloaded()
{
    // 3
    cout << "\t\t\t\t                               @@/@@@@               \n";
    cout << "\t\t\t\t       ,(*#@%(,,*, . ..*//(//@@#/@@@&(/(%%%%(#**#(@ ,\n";
    cout << "\t\t\t\t    &//%&%* .,*(,(*.,. ...@%&@@*&@@( .*/,,,//** .. ..\n";
    cout << "\t\t\t\t   &%%%&#.#/#(..,.,,(.*/*%%@.&%/%@@/.,%%%#%##,/(@    \n";
    cout << "\t\t\t\t (%&%@%&%,.. ..          %*@@,(*%/&@,     ,*@        \n";
    cout << "\t\t\t\t  /@%%%(,....             #/%@,*,,*&@@@@@&/*&&       \n";
    cout << "\t\t\t\t      #,...                .*,,,.,..       ./@@.     \n";
}
void imageGunReload()
{
    // 2
    cout << "\t\t\t\t       ,(*#@%(,,*, . ..*//(//@@#/@@@&(/(%%%%(#**#(@ ,\n";
    cout << "\t\t\t\t    &//%&%* .,*(,(*.,. ...@%&@@*&@@( .*/,,,//** .. ..\n";
    cout << "\t\t\t\t   &%%%&#.#/#(..,.,,(.*/*%%@.&%/%@@/.,%%%#%##,/(@    \n";
    cout << "\t\t\t\t (%&%@%&%,.. ..          %*@@,(*%/&@,     ,*@        \n";
    cout << "\t\t\t\t  /@%%%(,....             #/%@,*,,*&@@@@@&/*&&       \n";
    cout << "\t\t\t\t    ###,...                .*,,,.,..       ./@@.     \n";
    cout << "\t\t\t\t     ##,...                .*,,,.,...                \n";
}
void imageGunHold()
{
    // 1
    sleep(1);
    cout << "\t\t\t\t                               @@/@@@@               \n";
    usleep(20000);
    cout << "\t\t\t\t       ,(*#@%(,,*, . ..*//(//@@#/@@@&(/(%%%%(#**#(@ ,\n";
    usleep(20000);
    cout << "\t\t\t\t    &//%&%* .,*(,(*.,. ...@%&@@*&@@( .*/,,,//** .. ..\n";
    usleep(20000);
    cout << "\t\t\t\t   &%%%&#.#/#(..,.,,(.*/*%%@.&%/%@@/.,%%%#%##,/(@    \n";
    usleep(20000); // FADE-IN ANIMATION
    cout << "\t\t\t\t (%&%@%&%,.. ..          %*@@,(*%/&@,     ,*@        \n";
    usleep(20000);
    cout << "\t\t\t\t  /@%%%(,....             #/%@,*,,*&@@@@@&/*&&       \n";
    usleep(20000);
    cout << "\t\t\t\t      #,...                .*,,,.,..       ./@@.     \n";
    usleep(400000);
}
void imageGunShot()
{
    // 3
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t .,//&&*/*/,.               \n";
    cout << "\t\t\t\t\t\t\t\t ,*/&&&/(&&&&/*,.           \n";
    cout << "\t\t\t\t\t\t\t\t  ,.**/(&#&/&&&%%*,,..      \n";
    cout << "\t\t\t\t\t\t\t\t    ,**////&%&/&&&&&/,,..   \n";
    cout << "\t\t\t\t\t\t\t\t   ,*&*,.,***/#&%/&&&&(*,.. \n";
}
void imageGunShoot()
{
    // 2
    cout << "\t\t\t\t\t\t\t\t  .,//&&*/*/,.              \n";
    cout << "\t\t\t\t\t\t\t\t  ,*/&&&/(&&&&/*,.          \n";
    cout << "\t\t\t\t\t\t\t\t   ,.**/(&#&/&&&%%*,,..     \n";
    cout << "\t\t\t\t\t\t\t\t     ,**////&%&/&&&&&/,,..  \n";
    cout << "\t\t\t\t\t\t\t\t    ,*&*,.,***/#&%/&&&&(*,..\n";
    cout << "\t\t\t\t\t\t\t\t     *&#&&.  .**/((%%&#&&&/*\n";
}
void imageGunPoint()
{
    // 1
    sleep(1);
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t .,//&&*/*/,.               \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t ,*/&&&/(&&&&/*,.           \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t  ,.**/(&#&/&&&%%*,,..      \n";
    usleep(20000); // FADE-IN ANIMATION
    cout << "\t\t\t\t\t\t\t\t    ,**////&%&/&&&&&/,,..   \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t   ,*&*,.,***/#&%/&&&&(*,.. \n";
}
void imageGunShotSelf()
{
    // 3
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t   .,//(((((//*,.        \n";
    cout << "\t\t\t\t\t\t\t\t ./((*.       ((/,.      \n";
    cout << "\t\t\t\t\t\t\t\t ,((*.        .(((/*     \n";
    cout << "\t\t\t\t\t\t\t\t  ./(*.       (((,../*/, \n";
    cout << "\t\t\t\t\t\t\t\t   *.(((((((((,.. **,*.* \n";
}
void imageGunShootSelf()
{
    // 2
    cout << "\t\t\t\t\t\t\t\t    .,//(((((//*,.       \n";
    cout << "\t\t\t\t\t\t\t\t  ./((*.       ((/,.     \n";
    cout << "\t\t\t\t\t\t\t\t  ,((*.        .(((/*    \n";
    cout << "\t\t\t\t\t\t\t\t   ./(*.       (((,../*/,\n";
    cout << "\t\t\t\t\t\t\t\t    *.(((((((((,.. **,*.*\n";
    cout << "\t\t\t\t\t\t\t\t        ,(.  ..,.  //////\n";
}
void imageGunPointSelf()
{
    // 1
    sleep(1);
    cout << "\n";
    cout << "\t\t\t\t\t\t\t\t   .,//(((((//*,.        \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t ./((*.       ((/,.      \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t ,((*.        .(((/*     \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t  ./(*.       (((,../*/, \n";
    usleep(20000);
    cout << "\t\t\t\t\t\t\t\t   *.(((((((((,.. **,*.* \n";
}
void imageGunShootPlayer()
{
    cout << "\t\t\t\t\t\t\t\t//////  .,..  .(,      \n";
    cout << "\t\t\t\t\t\t\t\t*.*,** ..,(((((((((.*  \n";
    cout << "\t\t\t\t\t\t\t\t,/*/..,(((       .*(/. \n";
    cout << "\t\t\t\t\t\t\t\t    */(((.        .*((,\n";
    cout << "\t\t\t\t\t\t\t\t     .,/((       .*((/.\n";
    cout << "\t\t\t\t\t\t\t\t       .,*//(((((//,.  \n";
}

// ---------- ---------- ---------- (ANIMATIONS) ---------- ---------- ----------
// ANIMATION: Dialogues
void playDialogue1()
{
    clear();
    imageBot();
    imageGunHold();
    cout << "\n\n\t\t\t\t      [I insert the shell in an unknown order.]\n";
    dialogue1();
    // sleep(2);
}
void playDialogue2()
{
    clear();
    imageBot();
    imageGunReloaded();
    cout << "\n\n\t\t\t\t   [Shooting yourself with a blank skips my turn.]\n";
    dialogue2();
    sleep(1);
}
void playDialogue3()
{
    clear();
    imageBot();
    imageGunReloaded();
    cout << "\n\n\t\t\t\t [Shooting yourself with a blank skips their turn.]\n";
    dialogue3();
    sleep(2);
}
// ANIMAZTION: End Game (Victory & Gameover)
void victoryScreen()
{
    clear();
    victory();
    imageVictory();
    cout << "\n\t\t\t\t             PRESS ENTER TO CONTINUE...";
    cin.get(); // Waits for the user to press Enter
    loading();
}
void gameoverScreen()
{
    clear();
    gameover();
    imageGameover();
    cout << "\n\t\t\t\t             PRESS ENTER TO RESTART...";
    cin.get(); // Waits for the user to press Enter
    loading();
}
// ANIMATION: Holding Gun
void armedBot()
{
    clear();
    imageBot();
    imageGunHold();
}
void armedPlayer()
{
    clear();
    imagePlayer1();
    imageGunHold();
}
void armedPlayer2()
{
    clear();
    imagePlayer2();
    imageGunHold();
}
// ANIMATION: SOLO GAMEPLAY
void reloadBot(Level level)
{
    // Bot reloads gun
    for (int i = 0; i < level.numberOfBullets; i++)
    {
        clear();
        imageBot();
        imageGunReload();
        reload();
        Sleep(60);
        clear();
        imageBot();
        imageGunReloaded();
        Sleep(60);
    }
    Sleep(100);
}
void shootBot()
{ // Player shoots Bot (Live Bullet)
    clear();
    imageBot();
    imageGunPoint();
    clear();
    imageBotAngry();
    imageGunShoot();
    gunshot();
    clear();
    imageBotAngry();
    imageGunShot();
}
void shootBotBlank()
{ // Player shoots Bot (Blank Bullet)
    clear();
    imageBot();
    imageGunPoint();
    clear();
    imageBot();
    imageGunShoot();
    gunclick();
    clear();
    imageBot();
    imageGunShot();
}
void shootPlayer()
{ // Bot shoots Player (Live Bullet)
    clear();
    imagePlayer1();
    imageGunPoint();
    clear();
    imagePlayer1();
    imageGunShoot();
    gunshot();
    clear();
    imagePlayer1();
    imageGunShot();
}
void shootPlayerBlank()
{ // Bot shoots Player (Blank Bullet)
    clear();
    imagePlayer1();
    imageGunPoint();
    clear();
    imagePlayer1();
    imageGunShoot();
    gunclick();
    clear();
    imagePlayer1();
    imageGunShot();
}
void playerShootsSelf()
{ // Player shoots Self (Live Bullet)
    clear();
    imageBot();
    imageGunPointSelf();
    clear();
    imageBot();
    imageGunShootSelf();
    gunshot();
    clear();
    imageBot();
    imageGunShotSelf();
}
void playerShootsSelfBlank()
{ // Player shoots Self (Blank Bullet)
    clear();
    imageBot();
    imageGunPointSelf();
    clear();
    imageBot();
    imageGunShootSelf();
    gunclick();
    clear();
    imageBot();
    imageGunShotSelf();
}
void botShootsSelf()
{
    // Bot shoots Self (Live Bullet)
    clear();
    imagePlayer1();
    imageGunPointSelf();
    clear();
    imagePlayer1();
    imageGunShootSelf();
    gunshot();
    clear();
    imagePlayer1();
    imageGunShotSelf();
}
void botShootsSelfBlank()
{
    // Bot shoots Self (Blank Bullet)
    clear();
    imagePlayer1();
    imageGunPointSelf();
    clear();
    imagePlayer1();
    imageGunShootSelf();
    gunclick();
    clear();
    imagePlayer1();
    imageGunShotSelf();
}
// ANIMATION - MULTIPLAYER GAMEPLAY
void player1ShootsPlayer2()
{ // Player1 shoots Player2 (Live Bullet)
    clear();
    imagePlayer2();
    imageGunPoint();
    clear();
    imagePlayer2();
    imageGunShoot();
    gunshot();
    clear();
    imagePlayer2();
    imageGunShot();
}
void player1ShootsPlayer2Blank()
{ // Player1 shoots Player2 (Blank Bullet)
    clear();
    imagePlayer2();
    imageGunPoint();
    clear();
    imagePlayer2();
    imageGunShoot();
    gunclick();
    clear();
    imagePlayer2();
    imageGunShot();
}
void player1ShootsSelf()
{ // Player1 shoots Self (Live Bullet)
    clear();
    imagePlayer2();
    imageGunPointSelf();
    clear();
    imagePlayer2();
    imageGunShootSelf();
    gunshot();
    clear();
    imagePlayer2();
    imageGunShotSelf();
}
void player1ShootsSelfBlank()
{ // Player1 shoots Self (Blank Bullet)
    clear();
    imagePlayer2();
    imageGunPointSelf();
    clear();
    imagePlayer2();
    imageGunShootSelf();
    gunclick();
    clear();
    imagePlayer2();
    imageGunShotSelf();
}
void player2ShootsPlayer1()
{ // Player1 shoots Player2 (Live Bullet)
    clear();
    imagePlayer1();
    imageGunPoint();
    clear();
    imagePlayer1();
    imageGunShoot();
    gunshot();
    clear();
    imagePlayer1();
    imageGunShot();
}
void player2ShootsPlayer1Blank()
{ // Player1 shoots Player2 (Blank Bullet)
    clear();
    imagePlayer1();
    imageGunPoint();
    clear();
    imagePlayer1();
    imageGunShoot();
    gunclick();
    clear();
    imagePlayer1();
    imageGunShot();
}
void player2ShootsSelf()
{ // Player1 shoots Self (Live Bullet)
    clear();
    imagePlayer1();
    imageGunPointSelf();
    clear();
    imagePlayer1();
    imageGunShootSelf();
    gunshot();
    clear();
    imagePlayer1();
    imageGunShotSelf();
}
void player2ShootsSelfBlank()
{ // Player1 shoots Self (Blank Bullet)
    clear();
    imagePlayer1();
    imageGunPointSelf();
    clear();
    imagePlayer1();
    imageGunShootSelf();
    gunclick();
    clear();
    imagePlayer1();
    imageGunShotSelf();
}

// ---------- ---------- ---------- (ALL MENUS GUI) ---------- ---------- ----------
void option()
{
    // Display in-game options
    cout << "\t\t\t\t\t                OPTIONS           \n";
    usleep(50000);
    cout << "\t\t\t\t\t     -     1.Shoot Yourself      -\n";
    usleep(50000);
    cout << "\t\t\t\t\t     -   2.Shoot Your Opponent   -\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                INPUT:";
}

void optionWithSpecial()
{
    // Display in-game options (with SPECIALS)
    cout << "\t\t\t\t\t                OPTIONS           \n";
    usleep(50000);
    cout << "\t\t\t\t\t     -     1.Shoot Yourself      -\n";
    usleep(50000);
    cout << "\t\t\t\t\t     -   2.Shoot Your Opponent   -\n";
    usleep(50000);
    cout << "\t\t\t\t\t     -    3.Use Special Items    -\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                INPUT:";
}

void displaySpecialItemOption(Player &player)
{
    cout << "\t\t\t\t\t                OPTIONS           \n";
    usleep(50000);
    cout << "\t\t\t\t\t     -         1." << left << setw(16) << player.specialItem[0] << "-\n";
    usleep(50000);
    cout << "\t\t\t\t\t     -         2." << left << setw(16) << player.specialItem[1] << "-\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                INPUT:";
}

void searchOption()
{
    cout << "\n";
    cout << "\t\t\t\t\t                OPTIONS           \n";
    usleep(50000);
    cout << "\t\t\t\t\t       -       1.Search          -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -        0.Back           -\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                INPUT:";
}

void searchAlgorithmOption()
{ // NOTE: HANDLE ERROR PG
    cout << "\n";
    cout << "\t\t\t\t\t                 OPTIONS          \n";
    usleep(50000);
    cout << "\t\t\t\t\t       -     1.Linear Search     -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -     2.Binary Search     -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -   3.Binary Search Tree  -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -         0.Back          -\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                 INPUT:";
}

void searchLeaderboard(const string &file, Node *&head)
{
    char input;
    string name;

    searchOption();
    cin >> input;
    clearInputBuffer();

    if (input == '0')
    {
        rack();
        leaderboard();
    }
    else
    {
        rack();
        clear();
        titleStatic();
        // clearInputBuffer();
        displayDataFromFile(file, head);

        if (input == '1')
        {
            do
            {
                searchAlgorithmOption();
                cin >> input;
                clearInputBuffer();
        a:
                rack();
                clear();
                titleStatic();
                displayDataFromFile(file, head);

                switch (input)
                {
                case '0':
                    searchLeaderboard(file, head);
                    break;
                case '1':
                    cout << "\n\t\t\t\t           Search Type: Linear Search\n";
                    break;
                case '2':
                    cout << "\n\t\t\t\t           Search Type: Binary Search\n";
                    break;
                case '3':
                    cout << "\n\t\t\t\t        Search Type: Binary Search Tree\n";
                    break;
                }
            } while (input - '0' < 0 || input - '0' > 3);

            cout << "\n\t\t\t\t           Enter name to search: ";
            getline(cin, name);
            clearSpace(name);
            name = uppercaseConversion(name);

            if (!validateName(name) || name.empty())
                goto a;

            rack();
            clear();
            titleStatic();
            switch (input)
            {
            case '1':
                linearSearch(file, head, name);
                break;
            case '2':
                binarySearch(file, head, name);
                break;
            case '3':
                binarySearchTree(file, head, name);
                break;
            }
        }
        searchLeaderboard(file, head);
    }
}

void displayBullets(Level level)
{
    cout << "\t\t\t\t\t\t" << level.numberOfLiveBullets << " LIVE(S), " << (level.numberOfBullets - level.numberOfLiveBullets) << " BLANK(S)\n\n\n";
}

void displayLife(Player &player1, Player &player2)
{
    string s1 = player1.name + ": ";
    string s2 = player2.name + ": ";

    for (int i = 0; i < player1.life; i++)
        s1 += '#';

    for (int i = 0; i < player2.life; i++)
        s2 += '#';

    cout << "    " << s1 << "\t\t\t\t\t\t\t\t\t\t\t\t" << s2 << "\n";
    cout << "    ---------------------------------------------------------------------------------------------------------------   \n";
}

void specialItemOption()
{
    cout << "\t\t\t\t\t           1. Inspect Bullet             \n"
         << "\t\t\t\t\t           2. Skip Bullet                \n\n";
}

void credits()
{
    rack();
    clear();
    titleStatic();
    cout << "\n";
    cout << "\t\t\t\t\t                CREDITS \n\n";
    usleep(50000);
    cout << "\t\t\t\t\t      - ORIGINAL GAME and MUSIC -\n";
    usleep(50000);
    cout << "\t\t\t\t\t             Mike KLUBNIKA\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t       - PROJECT COLLABORATORS -\n";
    usleep(50000);
    cout << "\t\t\t\t\t            Chanreach TRY\n";
    usleep(50000);
    cout << "\t\t\t\t\t         Chansovisoth WATTANAK\n";
    usleep(50000);
    cout << "\t\t\t\t\t             Sodavith NUON\n";
    usleep(50000);
    cout << "\t\t\t\t\t                Rithy CHAN\n";
    usleep(50000);
    cout << "\t\t\t\t\t              Sovitep CHEA\n\n";
    usleep(100000);
    cout << "\t\t\t           This game is a fan-made university project inspired\n";
    usleep(50000);
    cout << "\t\t\t       by the original work of  Mike KLUBNIKA, \"Buckshot Roulette\"\n";
    usleep(50000);
    cout << "\t\t\t  It is created out of admiration and appreciation for the original game.\n\n";
    usleep(50000);

    cout << "\t\t\t\t\t        PRESS ENTER TO GO BACK...";
    cin.get(); // Waits for the user to press Enter
    start();
}

void leaderboard()
{
    // rack();
    clear();
    titleStatic();
    cout << "\n\n\n\n\n";
    cout << "\t\t\t\t\t              LEADERBOARD       \n";
    usleep(50000);
    cout << "\t\t\t\t\t       -      1.Solo Mode      -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -   2.Multiplayer Mode  -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -        0.Back         -\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                INPUT:";
    char input;
    cin >> input;
    clearInputBuffer();

    switch (input)
    {
    case '0':
        start();
        break;
    case '1':
        rack();
        displayDataFromFile(file1, head1);
        searchLeaderboard(file1, head1);
        break;
    case '2':
        rack();
        displayDataFromFile(file2, head2);
        searchLeaderboard(file2, head2);
        break;
    default:
        rack();
        leaderboard();
        break;
    }
}

void menu()
{
    cout << "\n\n\n\n\n";
    cout << "\t\t\t\t\t                  MENU          \n";
    usleep(50000);
    cout << "\t\t\t\t\t       -      1.Solo Mode      -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -   2.Multiplayer Mode  -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -     3.Leaderboard     -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -      4. Credits       -\n";
    usleep(50000);
    cout << "\t\t\t\t\t       -      0.Exit Game      -\n\n";
    usleep(50000);
    cout << "\t\t\t\t\t                 INPUT:";
}

void start()
{
    Player player1, player2, bot;
    char input;
    bool invalid = false;

    do
    {
        (invalid) ? warning() : rack();
        clear();
        titleStatic();
        menu();
        cin >> input;
        clearInputBuffer();

        switch (input)
        {
        case '0':
            clear();
            cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t - [ CLOSING GAME ] - \n\n\n\n\n\n\n\n\n\n\n\n\n";
            exitSound();
            exit(0);
            break;
        case '1':
            rack();
            loading();
            soloMode(player1, bot, file1);
            intro(); // TEST MUSIC
            break;
        case '2':
            rack();
            loading();
            multiplayerMode(player1, player2, file2);
            intro(); // TEST MUSIC
            break;
        case '3':
            rack();
            leaderboard();
            break;
        case '4':
            credits();
            break;
        default:
            invalid = true;
        }
        // intro(); // TEST COMMENTED

    } while (input != '0');
}

void titleScreen()
{
    intro_remix();
    Sleep(700); // Intentional titlle screen delay
    title();
    Sleep(1350);
    titleArt();
    cout << "\n\t\t\t\t             PRESS ENTER TO CONTINUE...";
    cin.get(); // Waits for the user to press Enter
               //	loading();
}

int botChoice(Player &bot, int numberOfOptions)
{
    return rand() % numberOfOptions + 1;
}

void randomizeSpecialItem(Player &player)
{
    string specialItem[] = {"INSPECT", "SKIP"};

    for (int i = 0; i < NUMBER_OF_SPECIAL_ITEMS; i++)
    {
        int randomItem = rand() % 2;
        player.specialItem[i] = specialItem[randomItem];
    }
}

void displaySpecialItem(Player &player)
{
    string str = player.name + "'s special items: ";
    if (player.numberOfItems <= 0)
        str += "NONE";
    else
        for (int i = 0; i < player.numberOfItems; i++)
            str += ((player.numberOfItems == 1) ? "" : to_string(i + 1) + ".") + player.specialItem[i] + " ";

    if (playTurn)
        cout << "" << left << setw(42) << str;
    else
        cout << str;
}

void deleteSpecialItem(Player &player, int select)
{
    for (int i = select; i < player.numberOfItems - 1; i++)
        player.specialItem[i] = player.specialItem[i + 1];
    player.numberOfItems--;
}

void specialItemOperation(Player &player, int specialItemIndex, string bullets[], int currentBulletIndex, bool &inspectStatus)
{
    if (player.specialItem[specialItemIndex] == "INSPECT")
    {
        cout << "\t\t\t\t\t     Current Bullet is " << bullets[currentBulletIndex] << endl;
        inspectStatus = true;
    }
    else
    {
        cout << "\t\t\t\t\t     " << player.name << " has removed the bullet." << endl;
        inspectStatus = false;
    }
}

void displayTurn(Player &player1, Player &player2, bool solo, bool player1Turn, const string &currentPlayerName)
{
    displayLife(player1, player2);

    if (solo)
    {
        if (!player1Turn)
            imageBot();
        else
            imagePlayer1();
    }
    else
    {
        if (!player1Turn)
            imagePlayer2();
        else
            imagePlayer1();
    }

    cout << "\n";
    cout << "    -----------------------------------------[      " << currentPlayerName << " TURN      ]-----------------------------------------   \n";
    cout << "\n";
}

void gameLevel(Player &player1, Player &player2, Level level, bool player1Turn)
{
    clear();
    bool p1T = player1Turn;
    bool solo = (player1.name == "DEALER" || player2.name == "DEALER");
c:
    string bullets[level.numberOfBullets];

    // Initialize all the bullets to "BLANK"
    for (int i = 0; i < level.numberOfBullets; i++)
        bullets[i] = "BLANK";

    // Insert the "LIVE" bullet(s) randomly (spin the bullets)
    for (int i = 0; i < level.numberOfLiveBullets; i++)
    {
    a:
        int randomBullet = rand() % level.numberOfBullets;
        if (bullets[randomBullet] == "LIVE")
            goto a;
        bullets[randomBullet] = "LIVE";
    }

    for (int i = 0; i < level.numberOfBullets; i++)
    {
        int index = rand() % level.numberOfBullets;
        string temp = bullets[i];
        bullets[i] = bullets[index];
        bullets[index] = temp;
    }

    int choice;
    int n = level.numberOfBullets;
    for (int i = 0; i < n; i++)
    {
    b:
        clear();
        // Add more spaces until player name reaches 8 characters (+2 more because of 'S)
        string currentPlayerName = ((player1Turn) ? player1.name : player2.name) + "'S";
        if (currentPlayerName.length() < 10)
        {
            int spacesToAdd = 10 - currentPlayerName.length();
            currentPlayerName += string(spacesToAdd, ' ');
        }

        displayTurn(player1, player2, solo, player1Turn, currentPlayerName);
        option();

        if (!player1Turn && solo)
        { // Check if it is BOT's turn
            choice = botChoice((player1.name == "DEALER" ? player1 : player2), 2);
            sleep(1);
            cout << choice << endl;
            sleep(2);
        }
        else
        {
            cin >> choice;
            clearInputBuffer();
        }

        (!player1Turn) ? (solo ? armedBot() : armedPlayer2()) : armedPlayer();

        switch (choice)
        {
        case 1:
            if (bullets[i] == "LIVE")
            {
                if (solo)
                {
                    (player1Turn) ? playerShootsSelf() : botShootsSelf();
                    cout << "\n\t\t\t    [" << (player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (player1Turn ? "yourself" : "himself") << " with a LIVE bullet]\n";
                }
                else
                {
                    (player1Turn) ? player1ShootsSelf() : player2ShootsSelf();
                    cout << "\n\t\t\t    [" << (player1Turn ? player1.name : player2.name) << " pulled the trigger on themselves with a LIVE bullet]\n";
                }

                (player1Turn) ? player1.life-- : player2.life--;
                player1Turn = !player1Turn;
            }
            else
            {
                if (solo)
                {
                    (player1Turn) ? playerShootsSelfBlank() : botShootsSelfBlank();
                    cout << "\n\t\t\t    [" << (player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (player1Turn ? "yourself" : "himself") << " with a BLANK bullet]\n";
                }
                else
                {
                    (player1Turn) ? player1ShootsSelfBlank() : player2ShootsSelfBlank();
                    cout << "\n\t\t\t    [" << (player1Turn ? player1.name : player2.name) << " pulled the trigger on themselves with a BLANK bullet]\n";
                }
                pause();
                continue;
            }
            break;
        case 2:
            player1Turn = !player1Turn; // Reverse player turn
            if (bullets[i] == "LIVE")
            {
                if (solo)
                {
                    (player1Turn) ? shootPlayer() : shootBot();
                    cout << "\n\t\t\t    [" << (!player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (!player1Turn ? "the " + player2.name : " you") << " with a LIVE bullet]\n";
                }
                else
                {
                    (player1Turn) ? player2ShootsPlayer1() : player1ShootsPlayer2();
                    cout << "\n\t\t\t    [" << (!player1Turn ? player1.name : player2.name) << " pulled the trigger on " << (!player1Turn ? player2.name : player1.name) << " with a LIVE bullet]" << endl;
                }
                (player1Turn) ? player1.life-- : player2.life--;
            }
            else
            {
                if (solo)
                {
                    (player1Turn) ? shootPlayerBlank() : shootBotBlank();
                    cout << "\n\t\t\t    [" << (!player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (!player1Turn ? " the " + player2.name : " you ") << " with a BLANK bullet]\n";
                }
                else
                {
                    (player1Turn) ? player2ShootsPlayer1Blank() : player1ShootsPlayer2Blank();
                    cout << "\n\t\t\t    [" << (!player1Turn ? player1.name : player2.name) << " pulled the trigger on " << (!player1Turn ? player2.name : player1.name) << " with a BLANK bullet]" << endl;
                }
                pause();
                continue;
            }
            break;
        default:
            goto b;
        }

        if (player1.life <= 0 || player2.life <= 0 || i == level.numberOfBullets)
            break;

        pause();
        // level.numberOfBullets--;
    }

    if (player2.life == 0)
    {
        cout << "\n\t\t\t\t\t     " << player1.name << " SURVIVED THIS LEVEL" << endl;
        player1.winCount++;
    }
    else if (player1.life == 0)
    {
        cout << "\n\t\t\t\t\t     " << player2.name << " SURVIVED THIS LEVEL" << endl;
        player2.winCount++;
    }
    else
    {
        clear();
        displayLife(player1, player2);
        cout << "\n\n\n\n\n\n\n\n\n";
        cout << "\t\t\t\t       OUT OF BULLETS, RELOADING GUN . . ." << endl;
        Sleep(1500);
        clear();
        displayLife(player1, player2);
        cout << "\n\n\n\n\n\n\n\n\n";
        displayBullets(level);
        cout << "\n\n";
        pause();
        reloadBot(level);
        player1Turn = p1T;
        goto c;
    }
    pause();
}

void specialLevel(Player &player1, Player &player2, Level level, bool player1Turn)
{ // Level with special items
    clear();
    bool p1T = player1Turn;
    bool solo = (player1.name == "DEALER" || player2.name == "DEALER");
c:
    cout << "\t\t\t\t  For this level, you get 2 special items each." << endl;
    specialItemOption();

    player1.numberOfItems = 2;
    player2.numberOfItems = 2;

    // Randomize special items for each player
    randomizeSpecialItem(player1);
    randomizeSpecialItem(player2);

    string bullets[level.numberOfBullets];

    // Initialize all the bullets to "BLANK"
    for (int i = 0; i < level.numberOfBullets; i++)
        bullets[i] = "BLANK";

    // Insert the "LIVE" bullet(s) randomly (spin the bullets)
    for (int i = 0; i < level.numberOfLiveBullets; i++)
    {
    a:
        int randomBullet = rand() % level.numberOfBullets;
        if (bullets[randomBullet] == "LIVE")
            goto a;
        bullets[randomBullet] = "LIVE";
    }

    for (int i = 0; i < level.numberOfBullets; i++)
    {
        int index = rand() % level.numberOfBullets;
        string temp = bullets[i];
        bullets[i] = bullets[index];
        bullets[index] = temp;
    }

    int choice;
    int botTurn = 0; // Track BOT's turn for special items (for BOT choice)
    bool inspectStatus = false;
    int n = level.numberOfBullets;

    for (int i = 0; i < n; i++)
    {
    b:
        clear();
        if (player1Turn || (player1.name != "DEALER" && player2.name != "DEALER"))
            inspectStatus = false;

        string currentPlayerName = ((player1Turn) ? player1.name : player2.name) + "'S";
        if (currentPlayerName.length() < 10)
        {
            int spacesToAdd = 10 - currentPlayerName.length();
            currentPlayerName += string(spacesToAdd, ' ');
        }

        displayTurn(player1, player2, solo, player1Turn, currentPlayerName);

        // Display player's special items
        cout << "     ";
        displaySpecialItem(player1);
        cout << "\t\t\t\t ";
        displaySpecialItem(player2);
        cout << "\n";
        optionWithSpecial();

        if (!player1Turn && solo)
        {
            if (botTurn < NUMBER_OF_SPECIAL_ITEMS)
            { // For the first 2 BOT's turn, BOT must use special items
                if (inspectStatus)
                { // If BOT chose to inspect the bullet, they have to decide whether to shoot self or the player
                    choice = (bullets[i] == "LIVE") ? 2 : 1;
                    inspectStatus = false;
                }
                else
                {
                    choice = 3;
                    botTurn++;
                }
            }
            else if (inspectStatus)
            {
                choice = (bullets[i] == "LIVE") ? 2 : 1;
                inspectStatus = false;
            }
            else
                choice = botChoice((player1.name == "DEALER" ? player1 : player2), 2);
            sleep(1);
            cout << choice << endl;
            sleep(2);
        }
        else
        {
            cin >> choice;
            clearInputBuffer();
        }

        if (choice == 3)
        {
            clear();
            displayTurn(player1, player2, solo, player1Turn, currentPlayerName);
            cout << "     ";
            displaySpecialItem(player1);
            cout << "\t\t\t\t ";
            displaySpecialItem(player2);
            cout << "\n";
        }
        else
            (!player1Turn) ? (solo ? armedBot() : armedPlayer2()) : armedPlayer();

        switch (choice)
        {
        case 1:
            if (bullets[i] == "LIVE")
            {
                if (solo)
                {
                    (player1Turn) ? playerShootsSelf() : botShootsSelf();
                    cout << "\n\t\t\t    [" << (player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (player1Turn ? "yourself" : "himself") << " with a LIVE bullet]\n";
                }
                else
                {
                    (player1Turn) ? player1ShootsSelf() : player2ShootsSelf();
                    cout << "\n\t\t\t    [" << (player1Turn ? player1.name : player2.name) << " pulled the trigger on themselves with a LIVE bullet]\n";
                }

                (player1Turn) ? player1.life-- : player2.life--;
                player1Turn = !player1Turn;
            }
            else
            {
                if (solo)
                {
                    (player1Turn) ? playerShootsSelfBlank() : botShootsSelfBlank();
                    cout << "\n\t\t\t    [" << (player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (player1Turn ? "yourself" : "himself") << " with a BLANK bullet]\n";
                }
                else
                {
                    (player1Turn) ? player1ShootsSelfBlank() : player2ShootsSelfBlank();
                    cout << "\n\t\t\t    [" << (player1Turn ? player1.name : player2.name) << " pulled the trigger on themselves with a BLANK bullet]\n";
                }
                pause();
                continue;
            }
            break;
        case 2:
            player1Turn = !player1Turn; // Reverse player turn
            if (bullets[i] == "LIVE")
            {
                if (solo)
                {
                    (player1Turn) ? shootPlayer() : shootBot();
                    cout << "\n\t\t\t    [" << (!player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (!player1Turn ? "the " + player2.name : " you") << " with a LIVE bullet]\n";
                }
                else
                {
                    (player1Turn) ? player2ShootsPlayer1() : player1ShootsPlayer2();
                    cout << "\n\t\t\t    [" << (!player1Turn ? player1.name : player2.name) << " pulled the trigger on " << (!player1Turn ? player2.name : player1.name) << " with a LIVE bullet]" << endl;
                }
                (player1Turn) ? player1.life-- : player2.life--;
            }
            else
            {
                if (solo)
                {
                    (player1Turn) ? shootPlayerBlank() : shootBotBlank();
                    cout << "\n\t\t\t    [" << (!player1Turn ? "You" : "The " + player2.name) << " pulled the trigger on " << (!player1Turn ? " the " + player2.name : " you ") << " with a BLANK bullet]\n";
                }
                else
                {
                    (player1Turn) ? player2ShootsPlayer1Blank() : player1ShootsPlayer2Blank();
                    cout << "\n\t\t\t    [" << (!player1Turn ? player1.name : player2.name) << " pulled the trigger on " << (!player1Turn ? player2.name : player1.name) << " with a BLANK bullet]" << endl;
                }
                pause();
                continue;
            }
            break;
        case 3:
        d:
            clear();
            displayTurn(player1, player2, solo, player1Turn, currentPlayerName);
            cout << "     ";
            displaySpecialItem(player1);
            cout << "\t\t\t\t ";
            displaySpecialItem(player2);
            cout << "\n";

            if (((player1Turn) ? player1.numberOfItems : player2.numberOfItems) <= 0)
            { // Check special items of the player turn
                cout << "\n\t\t\t\t\t       YOU ARE OUT OF ITEMS" << endl;
                pause();
                goto b;
            }
            else
            {
                int numberOfItems = (player1Turn) ? player1.numberOfItems : player2.numberOfItems;
                int select;
                if (numberOfItems == 1)
                    select = 1;
                else
                {
                    displaySpecialItemOption(player1Turn ? player1 : player2);
                    if (!player1Turn && solo)
                    {
                        select = botChoice((player1.name == "DEALER") ? player1 : player2, 2);
                        sleep(2);
                        cout << select << endl;
                        sleep(1);
                    }
                    else
                    {
                        cin >> select;
                        clearInputBuffer();

                        if (select != 1 && select != 2)
                            goto d;
                    }
                }

                clear();
                displayTurn(player1, player2, solo, player1Turn, currentPlayerName);
                cout << "     ";
                displaySpecialItem(player1);
                cout << "\t\t\t\t ";
                displaySpecialItem(player2);
                cout << "\n";
                specialItemOperation((player1Turn) ? player1 : player2, select - 1, bullets, i, inspectStatus); // Perform selected special item
                deleteSpecialItem((player1Turn) ? player1 : player2, select - 1);                               // Remove the selected special item
                Sleep(1500);

                if (inspectStatus)
                    goto b;
                else
                    continue;
            }
            break;
        default:
            goto b;
        }

        if (player1.life <= 0 || player2.life <= 0 || i == level.numberOfBullets)
            break;

        pause();
        // level.numberOfBullets--;
    }

    if (player2.life == 0)
    {
        cout << "\n\t\t\t\t\t     " << player1.name << " SURVIVED THIS LEVEL" << endl;
        player1.winCount++;
    }
    else if (player1.life == 0)
    {
        cout << "\n\t\t\t\t\t     " << player2.name << " SURVIVED THIS LEVEL" << endl;
        player2.winCount++;
    }
    else
    {
        clear();
        displayLife(player1, player2);
        cout << "\n\n\n\n\n\n\n\n\n";
        cout << "\t\t\t\t       OUT OF BULLETS, RELOADING GUN . . ." << endl;
        Sleep(1500);
        clear();
        displayLife(player1, player2);
        cout << "\n\n\n\n\n\n\n\n\n";
        displayBullets(level);
        cout << "\n\n";
        pause();
        reloadBot(level);
        player1Turn = p1T;
        goto c;
    }
    pause();
}

void gamePlay(Player &player1, Player &player2, bool player1Turn)
{
    bool solo = (player1.name == "DEALER" || player2.name == "DEALER");
    (solo) ? music1() : stopMusic();

    for (int i = 0; i < MAXIMUM_OF_LEVELS; i++)
    {
        if ((player1.winCount == 3 || player2.winCount == 3))
            break;

        if (i == 4)
        {
            cout << "    TIE GAME" << endl;
            cout << "    LOADING FINAL LEVEL" << endl;
        }

        player1.life = levels[i].numberOfLife;
        player2.life = levels[i].numberOfLife;

        clear();

        displayLife(player1, player2);
        cout << "\n\n\n\n\n\n\n";
        cout << "\t\t\t\t\t\t\tLEVEL " << i + 1 << "\n\n"; // Display level
        displayBullets(levels[i]);
        pause();

        if (i == 0)
        {
            playDialogue1();
            (solo) ? (void)0 : music2();
            reloadBot(levels[i]);
            (solo) ? playDialogue2() : playDialogue3();
        }
        else
            reloadBot(levels[i]);

        if (i > 1)
            specialLevel(player1, player2, levels[i], player1Turn);
        else
            gameLevel(player1, player2, levels[i], player1Turn);
    }

    player1.matches++;
    player2.matches++;

    (player1.winCount > player2.winCount) ? player1.wins++ : player2.wins++;

    player1.winPercentage = formatNumber(player1.wins * 100.0 / player1.matches);
    player2.winPercentage = formatNumber(player2.wins * 100.0 / player2.matches);
}

void playTurn(Player &player1, Player &player2)
{
    thud();
    char firstPlay;
    while (firstPlay != '1' && firstPlay != '2')
    {
        if (player1.name != "DEALER" && player2.name != "DEALER")
        {
            cout << "\n\t\t\t\t     Randomizing player turn. . ." << endl;
            firstPlay = rand() % 2 + 1 + '0';

            cout << "\n\t\t\t\t     " << ((firstPlay == '1') ? player1.name : player2.name) << " goes first" << endl;
            // clear();
            // thud();
            sleep(2);
            break;
        }
    b:
        cout << "\n\t\t\t    Do you want to play first or let your opponent play first?"
             << "\n\t\t\t    Enter 1 if you want to play first, otherwise 2: ";
        cin >> firstPlay;
        clearInputBuffer();
        cout << "\n";

        firstPlay = toupper(firstPlay);

        if (firstPlay != '1' && firstPlay != '2')
        {
            warning();
            clear();
            cout << "\t\t\t\t\t\t       SOLO MODE        \n";
            cout << "\n\t\t\t\t     Enter your name (8 char or less): " << player1.name << endl;
            goto b;
        }
        clear();
        thud();
        sleep(2);
    }

    gamePlay(player1, player2, (firstPlay == '1') ? true : false);
}

void soloMode(Player &player, Player &bot, const string &file)
{
    bot.name = "DEALER";

    stopMusic();
    thud();
    do
    {
        clear();
        cout << "\t\t\t\t\t\t       SOLO MODE        \n";
        cout << "\n\t\t\t\t     Enter your name (8 char or less): ";
        getline(cin, player.name);
        clearSpace(player.name);

        if (!validateName(player.name))
            warning();
    } while (!validateName(player.name));
    player.name = uppercaseConversion(player.name);

    playTurn(player, bot);

    clear();
    if (player.winCount < bot.winCount)
    {
        stopMusic();
        death();
        gameover();
        imageGameover();
    }
    else
    {
        victory();
        imageVictory();
        cout << "\n\t\t\t\t\t      " << player.name << " WINS THE GAME" << endl;
    }

    insertNode(head1, player);
    insertDataToFile(file1, head1);

    player.reset();
    bot.reset();

    pause();
}

void multiplayerMode(Player &player1, Player &player2, const string &file)
{
    stopMusic();
    thud();
    do
    {
        clear();
        cout << "\t\t\t\t\t\t   MULTIPLAYER MODE        \n";
        cout << "\n\t\t\t\t     Enter Player 1's name (8 char or less): ";
        getline(cin, player1.name);
        clearSpace(player1.name);

        if (!validateName(player1.name))
            warning();
    } while (!validateName(player1.name) || player1.name.empty());

    do
    {
        cout << "\t\t\t\t     Enter Player 2's name (8 char or less): ";
        getline(cin, player2.name);
        clearSpace(player2.name);
        if (!validateName(player2.name) || uppercaseConversion(player2.name) == uppercaseConversion(player1.name))
        {
            if (!validateName(player1.name))
                warning();
            clear();
            cout << "\t\t\t\t\t\t   MULTIPLAYER MODE        \n";
            cout << "\n\t\t\t\t     Enter Player 1's name (8 char or less): " << player1.name << endl;
        }

    } while (uppercaseConversion(player2.name) == uppercaseConversion(player1.name) || !validateName(player2.name));

    player2.name = uppercaseConversion(player2.name);
    player1.name = uppercaseConversion(player1.name);

    playTurn(player1, player2);
    clear();
    imageVictory();
    cout << "\n\t\t\t\t\t      " << ((player1.winCount > player2.winCount) ? player1.name : player2.name) << " WINS THE GAME" << endl;

    insertNode(head2, player1);
    insertNode(head2, player2);

    insertDataToFile(file2, head2);

    player1.reset();
    player2.reset();

    pause();
}