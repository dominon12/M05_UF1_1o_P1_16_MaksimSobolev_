#include "Map.h"
#include "TimeManager.h"
#include "Player.h"


/// <summary>
/// Sets the needed variables
/// </summary>
void Setup();
/// <summary>
/// Handles the inputs
/// </summary>
void Input();
/// <summary>
/// Handles the logic of the game
/// </summary>
void Logic();
/// <summary>
/// Draws the screen
/// </summary>
void Draw();

Map pacman_map = Map();
std::vector<Enemy> enemies;
Player player = Player(pacman_map.spawn_player)
USER_INPUTS input = USER_INPUTS::NONE;
bool run = true;
bool win = false;

int main()
{
    TimeManager::getInstance().variable = 0;

    Setup();
    while (run)
    {
        Input();
        Logic();
        Draw();
    }
}

void Setup()
{
    std::cout.sync_with_stdio(false);
    srand(time(NULL));

    int enemy_num = 0;
    std::cout << "Cuantos enemigos quieres?";
    std::cin >> enemy_num;

    for (size_t i = 0; i < enemy_num; i++)
    {
        enemies.push_back(Enemy(pacman_map.spawn_enemy));
    }
}

void Input()
{
    input = USER_INPUTS::NONE;
    if (ConsoleUtils::KeyPressed(VK_UP) || ConsoleUtils::KeyPressed('W'))
    {
        input = USER_INPUTS::UP;
    }
    if (ConsoleUtils::KeyPressed(VK_DOWN) || ConsoleUtils::KeyPressed('S'))
    {
        input = USER_INPUTS::DOWN;
    }
    if (ConsoleUtils::KeyPressed(VK_RIGHT) || ConsoleUtils::KeyPressed('D'))
    {
        input = USER_INPUTS::RIGHT;
    }
    if (ConsoleUtils::KeyPressed(VK_LEFT) || ConsoleUtils::KeyPressed('A'))
    {
        input = USER_INPUTS::LEFT;
    }
    if (ConsoleUtils::KeyPressed(VK_ESCAPE) || ConsoleUtils::KeyPressed('Q'))
    {
        input = USER_INPUTS::QUIT;
    }
}

void Logic()
{
    if (win)
    {
        switch (input)
        {
        case QUIT:
            run = false;
            break;
        }
    }
    else
    {
        if (input == USER_INPUTS::QUIT) run = false;

        player.Update(&pacman_map, input);

        for (size_t i = 0; i < enemies.size(); i++) 
        {
            Enemy::ENEMY_STATE enemy1State = enemies[i]::Update(&pacman_map, player.position);

            switch (enemy1State)
            {
                case Enemy::ENEMY_KILLED: 
                    player.points += 50;
                    break;
                case Enemy::ENEMY_DEAD:
                    player.position = pacman_map.spawn_player;
                    break;        
                default:
                    break;
            }
        }

        if (pacman_map.points <= 0)
        {
            win = true;
        }
    }
}

void Draw()
{
    ConsoleUtils::Console_SetPos(0,0);
    pacman_map.Draw();

    player.Draw();

    for (size_t i = 0; i < enemies.size(); i++) 
    {
        enemies[i].Draw();
    } 

    ConsoleUtils::Console_ClearCharacter({ 0,(short)pacman_map.Height });
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::CYAN);
    std::cout << "Puntuacion actual: " << player.points << " Puntuacion pendiente: " << pacman_map.points << std::endl;
    if (win)
    {
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::GREEN);
        std::cout << "Has ganado!" << std::endl;
    }

    TimeManager::getInstance().NextFrame();
}