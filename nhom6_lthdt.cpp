#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>

using namespace std;

enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    AQUA = 3,
    RED = 4,
    PURPLE = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9
};

// Forward declaration: (Colour handling)
void setColor(ConsoleColor color);
void resetColor();
void printText(const string& text, ConsoleColor color);

void userInput();

// ===== Class Character =====
class Character {
protected:
    int Health, MaxHealth;
    int Strength;
    int Magic;
    int Potions;
    int Exp;  // Thêm EXP vào lớp Character
    int Level; // Thêm Level vào lớp Character
    int SkillPoints, MaxSkillPoints; // Giới hạn lược sử dụng skill


public:
    virtual ~Character() {}
    //Character() : Exp(0), Level(1) {}  // Khởi tạo EXP và Level
    Character(int maxHP, int maxSP) : Health(maxHP), MaxHealth(maxHP), Strength(10), Magic(10), Potions(3), Exp(0), Level(1), SkillPoints(maxSP), MaxSkillPoints(maxSP) {}

    // ---------------- Character's getters -----------------------
    int getHealth() const {
        return Health;
    }
    int getStrength() const {
        return Strength;
    }
    int getMagic() const {
        return Magic;
    }
    int getPotions() const {
        return Potions;
    }
    int getExp() const {
        return Exp;
    }
    int getLevel() const {
        return Level;
    }
    int getSkillPoints() const {
        return SkillPoints;
    }

    // ---------------- Character's methods -----------------------
    void takeDamage(int D) {
        Health -= D;
    }

    void usePotion() {
        if (Potions > 0) {
            Potions--;
            int healthRestored = MaxHealth * 0.3;
            int skillPointsRestored = ceil(MaxSkillPoints * 0.3);

            Health += healthRestored;
            if (Health > MaxHealth) { Health = MaxHealth; }

            SkillPoints += skillPointsRestored;
            if (SkillPoints > MaxSkillPoints) { SkillPoints = MaxSkillPoints; }

            setColor(LIGHT_BLUE);
            cout << "You used a potion! Health and Skill Points slightly restored.\n";
            cout << "Remaining potions: " << Potions << ".\n";
            resetColor();
        } else {
            printText("No potions left! Turn lost.\n", RED);
            // cout << "No potions left! Turn lost.\n";
        }
    }

    virtual void replenishHealth() { // Hồi đầy máu
        Health = MaxHealth;
        cout << "Health replenished to " << Health << "!\n";
    }
    virtual void replenishSkillPoints() { // Hồi đầy skill point
        SkillPoints = MaxSkillPoints;
        cout << "Skill Points replenished to " << SkillPoints << "!\n";
    }
    //gain exp, level up to heal 100%
    virtual void gainExp(int value) {
        Exp += value;
        while (Exp >= 100) {  // Level up khi EXP đạt 100 hoặc hơn
            Exp -= 100;
            Level++;
            cout << "Level Up! You are now level " << Level << "!" << endl;

            replenishHealth(); // Refill health after leveling up
            replenishSkillPoints(); // Refill Skill Points after leveling up

            // Heal player stats and increase them after leveling up
            Strength += Strength * 0.2;  // Increase Strength by 20%
            Magic += Magic * 0.2;  // Increase Magic by 20%

            // Potions increase by 1 for every level-up
            Potions++;

            cout << "Stats increased by 20%! Strength: " << Strength << ", Magic: " << Magic << ", Health: " << Health << endl;
        }
    }

    virtual void showStats() const {
        cout << "Health: " << Health << " | Strength: " << Strength << " | Magic: " << Magic << " | Level: " << Level << " | Skill Points: " << SkillPoints << endl;
    }

    virtual int action() = 0;  // Phương thức hành động ảo cho các lớp kế thừa
    virtual int useSkill(int skillChoice) = 0;
};

// ===== Class Knight =====
class Knight : public Character {
public:
    Knight() : Character(120, 5) {
        Health = 120;
        Strength = 15;
        Magic = 0;
        Potions = 3;
    }

    void replenishHealth() override {
        Health = MaxHealth; // Restore Health
    }
    void replenishSkillPoints() override {
        SkillPoints = MaxSkillPoints; // Restore Skill points
    }

    int useSkill(int skillChoice) override {
        if (SkillPoints <= 0) { // Kiểm tra nhân vật còn điểm SP không
            printText("Not enough Skill Points! Turn lost.\n", RED);
            // cout << "Not enough Skill Points! Turn lost.\n";
            return 0;
        }

        SkillPoints--; // Giam 1 SP sau moi luoc dung skill

        // Ultimate Skill: Last Resort

        switch (skillChoice) {
        case 1: // Power Slash
            printText("You use Power Slash!", LIGHT_BLUE);
            // cout << "You use Power Slash!" << endl;
            return Strength * 2;
        case 2: // Shield Block
            printText("You use Shield Block! You can now take more damage.", LIGHT_BLUE);
            // cout << "You use Shield Block! You can now take more damage." << endl;
            Health += 37;
            return 0;
        case 3: // Charge
            printText("You use Charge! Strength increases.", LIGHT_BLUE);
            // cout << "You use Charge! Strength increases." << endl;
            Strength += 5;
            return Strength * 0.1; // Moderate damage
        case 4: // last Resort
            printText("You use Last Resort!", LIGHT_BLUE);
            // cout << "You use Last Resort!" << endl;
            SkillPoints = 0;
            return Strength * 50 / Health;
        default:
            printText("You don't know how to use that! Turn lost.", RED);
            // cout << "You don't know how to use that! Turn lost." << endl;
            return 0;
        }
    }

    int action() override {
        // Knight tấn công với kiếm hoặc sử dụng potion
        cout << "Choose your action:\n1. Attack\n2. Use Potion\n3. Use Skills\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "You attack with your sword!" << endl;

            // SP increases by 1 each turn.
            if (SkillPoints < MaxSkillPoints) {
                SkillPoints += 1;
            } else {
                SkillPoints += 0;
            }
            return Strength;
        }
        else if (choice == 2 && Potions > 0) {
            usePotion();
            return 0;
        } else if (choice == 3){
            cout << "Knight skills: " << endl;
            cout << "1. Power Slash\n2. Shield Block\n3. Charge\n4. Last Resort (The less health you have, the more damage you'll deal)\n";
            int skillChoice;
            cin >> skillChoice;
            return useSkill(skillChoice);
        } else {
            printText("Invalid choice or out of potions!", RED);
            // cout << "Invalid choice or out of potions!" << endl;
            return 0;
        }
    }

    void showStats() const override {
        cout << "Knight Stats - ";
        Character::showStats();
    }
};

// ===== Class Wizard =====
class Wizard : public Character {
public:
    Wizard() : Character(100, 10) {
        Health = 100;
        Strength = 5;
        Magic = 20;
        Potions = 3;
    }

    void replenishHealth() override {
        Health = MaxHealth; // Restore Health
    }
    void replenishSkillPoints() override {
        SkillPoints = MaxSkillPoints; // Restore Skill points
    }

    int useSkill(int skillChoice) override {
        if (SkillPoints <= 0) { // Kiểm tra nhân vật còn điểm SP không
            printText("Not enough Skill Points! Turn lost.\n", RED);
            // cout << "Not enough Skill Points! Turn lost.\n";
            return 0;
        }

        SkillPoints--; // Giam 1 SP sau moi luoc dung skill

        // Ultimate skill: The Light Of Justice

        switch (skillChoice) {
        case 1: // Fire ball
            printText("You cast Fireball!", LIGHT_BLUE);
            // cout << "You cast Fireball!" << endl;
            return Magic * 2;
        case 2: // Magic Shield
            printText("You cast Magic Shield! You can now take more damage.", LIGHT_BLUE);
            // cout << "You cast Magic Shield! You can now take more damage." << endl;
            Health += 10;
            return 0;
        case 3: // Arcane Surge
            printText("You use Arcane Surge! Magic increases.", LIGHT_BLUE);
            // cout << "You use Arcane Surge! Magic increases." << endl;
            Magic += 14;
            return 0;
        case 4: // The Light of Justice
            printText("You use The Light of Justice!", LIGHT_BLUE);
            // cout << "You use The Light of Justice!" << endl;
            if (SkillPoints == 1) {
                SkillPoints = 0;
                return Magic * MaxSkillPoints;
            } else {
                printText("Fail to use! (Your SP must precisely be at 1 point to use)", RED);
                // cout << "Fail to use! (Your SP must precisely be at 1 point to use)" << endl;
                return 0;
            }
            return 0;
        default:
            printText("You don't know how to use that! Turn lost.", RED);
            // cout << "You don't know how to use that! Turn lost." << endl;
            return 0;
        }
    }

    int action() override {
        // Wizard tấn công bằng phép thuật hoặc sử dụng potion
        cout << "Choose your action:\n1. Attack\n2. Use Potion\n3. Use Skills\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "You hit your opponent with your wand!" << endl;

            // SP increases by 1 each turn.
            if (SkillPoints < MaxSkillPoints) {
                SkillPoints += 1;
            } else {
                SkillPoints += 0;
            }
            return Strength;
        }
        else if (choice == 2 && Potions > 0) {
            usePotion();
            return 0;
        } else if (choice == 3) {
            cout << "Wizard skills: " << endl;
            cout << "1. Fire ball\n2. Magic Shield\n3. Arcane Surge\n4. The Light of Justice (Your SP must precisely be at 1 point to use)\n";
            int skillChoice;
            cin >> skillChoice;
            return useSkill(skillChoice);
        }
        else {
            printText("Invalid choice or out of potions!", RED);
            // cout << "Invalid choice or out of potions!" << endl;
            return 0;
        }
    }
    void showStats() const override {
        cout << "Wizard Stats - ";
        Character::showStats();
    }
};

// ===== Class Assassin =====
class Assassin : public Character {
public:
    Assassin() : Character(80, 7) {
        Health = 80;
        Strength = 20;
        Magic = 0;
        Potions = 3;
    }

    void replenishHealth() override {
        Health = MaxHealth; // Restore Health
    }
    void replenishSkillPoints() override {
        SkillPoints = MaxSkillPoints; // Restore Skill points
    }

    int useSkill(int skillChoice) override {
        if (SkillPoints <= 0) { // Kiểm tra nhân vật còn điểm SP không
            printText("Not enough Skill Points! Turn lost.\n", RED);
            // cout << "Not enough Skill Points! Turn lost.\n";
            return 0;
        }

        SkillPoints--; // Giam 1 SP sau moi luoc dung skill
        int ConsumedSkillPoints;

        switch (skillChoice) {
        case 1: // Deep Cut
            printText("You use Deep Cut!", LIGHT_BLUE);
            // cout << "You use Deep Cut!" << endl;
            return Strength * 2;
        case 2: // Dodge
            printText("You use Dodge!", LIGHT_BLUE);
            // cout << "You use Dodge!" << endl;
            if (Health < MaxHealth * 0.2) {
                Health *= Strength;
            } else {
                Health += Strength;
            }
            return 0;
        case 3: // One with the shadow
            printText("You hide in some corners to prepare your trick.", LIGHT_BLUE);
            // cout << "You hide in some corners to prepare your trick." << endl;
            SkillPoints += 3;
            return 0;
        case 4: // Stealth
            printText("You use Stealth.", LIGHT_BLUE);
            // cout << "You use Stealth." << endl;
            ConsumedSkillPoints = SkillPoints + 1;
            SkillPoints = 0;
            return Strength * 0.3 * ConsumedSkillPoints;
        default:
            printText("You don't know how to use that! Turn lost.", RED);
            // cout << "You don't know how to use that! Turn lost." << endl;
            return 0;
        }
    }

    int action() override {
        // Knight tấn công với kiếm hoặc sử dụng potion
        cout << "Choose your action:\n1. Attack\n2. Use Potion\n3. Use Skills\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "You attack with your hidden blade!" << endl;

            // SP increases by 1 each turn.
            if (SkillPoints < MaxSkillPoints) {
                SkillPoints += 1;
            } else {
                SkillPoints += 0;
            }
            return Strength;
        }
        else if (choice == 2 && Potions > 0) {
            usePotion();
            return 0;
        } else if (choice == 3){
            cout << "Assassin skills: " << endl;
            cout << "1. Deep Cut\n2. Dodge (use when your HP is low enough)\n3. One with the shadow\n4. Stealth\n";
            int skillChoice;
            cin >> skillChoice;
            return useSkill(skillChoice);
        } else {
            printText("Invalid choice or out of potions!", RED);
            // cout << "Invalid choice or out of potions!" << endl;
            return 0;
        }
    }

    void showStats() const override {
        cout << "Assassin Stats - ";
        Character::showStats();
    }
};

// ===== Class Dracula =====
class Dracula : public Character {
private:
    int BloodFrenzy;
public:
    Dracula() : Character(100, 0), BloodFrenzy(0) { // Initialize Blood Frenzy to 0
        Health = 100;
        Strength = 17;
        Magic = 10;
        Potions = 3;
    }

    void replenishHealth() override {
        Health = MaxHealth; // Restore Health
    }
    void replenishSkillPoints() override {
        SkillPoints = MaxSkillPoints; // Restore Skill points
    }

    int useSkill(int skillChoice) override {
        int ConsumedHealth = 0;

        /*
        if (SkillPoints <= 0) { // Kiểm tra nhân vật còn điểm SP không
            cout << "Not enough Skill Points! Turn lost.\n";
            return 0;
        }
        */

        // SkillPoints--;  Dracula doesn't need skill points

        // Ultimate Skill: Blood Feast

        switch (skillChoice) {
        case 1: // Blood Sweep
            printText ("You use Blood Sweep! You used your blood to attack the enemy.", LIGHT_BLUE);
            // cout << "You use Blood Sweep! ";
            ConsumedHealth = Health * 0.1;
            Health -= ConsumedHealth;
            // cout << "You used your blood to attack the enemy." << endl;
            return ConsumedHealth + Strength * 0.8;
        case 2: // Blood Suck
            BloodFrenzy++;
            setColor(LIGHT_BLUE);
            cout << "You use Blood Suck! You absorb enemy's health. BloodFrenzy increases by 1 (Current: " << BloodFrenzy << ")." << endl;
            resetColor();
            Health += MaxHealth * 0.2;

            // avoid over absorbing...
            if (Health > MaxHealth) {
                Health = MaxHealth;
            }
            return MaxHealth * 0.2 + Magic;
        case 3: // Blood Burst
            // condition to use the skill
            if (BloodFrenzy >= 3) {
                printText("You use Blood Burst! A blood orb formed and exploded.", LIGHT_BLUE);
                // cout << "You use Blood Burst! A blood orb formed and exploded." << endl;
                BloodFrenzy = 0;
                ConsumedHealth = Health;
                Health -= Health * 0.9;
                return ConsumedHealth;
            } else {
                printText("Fail to use, Blood Frenzy insufficient! Turn lost.", RED);
                // cout << "Fail to use, Blood Frenzy insufficient! Turn lost." << endl;
                return 0;
            }
        case 4: // Blood Feast
            //condition to use the skill
            if (Health < MaxHealth * 0.2) {
                printText("You use Blood Feast! You violently absorb all enemy's blood.", LIGHT_BLUE);
                // cout << "You use Blood Feast! You violently absorb all enemy's blood." << endl;
                Health += MaxHealth * 0.7 + Magic;
                return MaxHealth * 0.7;
            } else {
                printText("You are not craving for blood yet! Turn lost.", RED);
                // cout << "You are not craving for blood yet! Turn lost." << endl;
                return 0;
            }


        default:
            printText("You don't know how to use that! Turn lost.", RED);
            // cout << "You don't know how to use that! Turn lost." << endl;
            return 0;
        }
    }

    int action() override {
        cout << "Choose your action:\n1. Attack\n2. Use Potion\n3. Use Skills\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "You attack with your teeth! Your health is restored a bit." <<endl;

            // SP increases by 1 each turn.

            if (SkillPoints < MaxSkillPoints) {
                SkillPoints += 1;
            } else {
                SkillPoints += 0;
            }

            // HP increases by 5% each turn
            if (Health < MaxHealth) {
                Health += MaxHealth * 0.05;
            } else {
                Health = MaxHealth;
            }

            return Strength;
        }
        else if (choice == 2 && Potions > 0) {
            usePotion();
            return 0;
        } else if (choice == 3){
            cout << "Dracula skills: " << endl;
            cout << "1. Blood Sweep\n2. Blood Suck\n3. Blood Burst (3 Blood Frenzy required!)\n4. Blood Feast\n";
            int skillChoice;
            cin >> skillChoice;
            return useSkill(skillChoice);
        } else {
            printText("Invalid choice or out of potions!", RED);
            // cout << "Invalid choice or out of potions!" << endl;
            return 0;
        }
    }

    void showStats() const override {
        cout << "Dracula Stats - ";
        Character::showStats();
    }
};

// ===== Class Enemy =====
class Enemy {
public:
    string Name;
    int Health;
    int Damage;
    Enemy(string name, int health, int damage)
        : Name(name), Health(health), Damage(damage) {}

    virtual ~Enemy() {}

    string getName() const {
        return Name;
    }

    int getHealth() const {
        return Health;
    }

    void takeDamage(int D) {
        Health -= D;
    }

    int attack() const {
        cout << Name << " attacks for " << Damage << " damage!" << endl;
        return Damage;
    }

    void increaseHealth(int amount) {
        Health += amount;
    }

    void showStats() const {
        cout << Name << " Stats - Health: " << Health << " | Damage: " << Damage << endl;
    }
};

// Colour handling...
void setColor(ConsoleColor color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
void resetColor() {
    setColor(WHITE);
}
void printText(const string& text, ConsoleColor color) {
    setColor(color);
    cout << text << endl;
    resetColor();
}

// ===== Function: battle PVE) =====
void pveStatus(Character& player, Enemy& enemy) {
    // Show health status
    system("cls");
    setColor(YELLOW);
    cout << "\n------ Status Update -------\n";
    player.showStats();
    enemy.showStats();
    cout << "----------------------------\n";
    resetColor();
}
void pvpStatus(Character& player1, Character& player2) {
    system("cls");
    setColor(YELLOW);
    cout << "----------------------------\n";
    cout << "Player 1 HP: " << player1.getHealth() << " | Skill Points: " << player1.getSkillPoints() << endl;
    cout << "Player 2 HP: " << player2.getHealth() << " | Skill Points: " << player2.getSkillPoints() << endl;
    cout << "----------------------------\n";
    resetColor();
}
void userInput() {
    // Wait for user input before the next round
    cout << "Press Enter to continue...\n";
    cin.ignore(); // Clear the newline character from previous input
    cin.get();    // Wait for the user to press Enter
}


void battlePVE(Character& player, Enemy& enemy) {
    // Declare the monster type before initiating the battle
    cout << "Battle Started!" << endl;

    userInput();

    pveStatus(player, enemy); // Show status

    // Battle loop
    while (player.getHealth() > 0 && enemy.Health > 0) {
        pveStatus(player, enemy); // Show status

        // Player's turn
        cout << "Player's turn:\n";
        int damage = 0;
        if (auto knight = dynamic_cast<Knight*>(&player)) {
            damage = knight->action();
        }
        else if (auto wizard = dynamic_cast<Wizard*>(&player)) {
            damage = wizard->action();
        }
        else if (auto assassin = dynamic_cast<Assassin*>(&player)) {
            damage = assassin->action();
        }
        else if (auto dracula = dynamic_cast<Dracula*>(&player)) {
            damage = dracula->action();
        }
        // Apply damage to the enemy
        enemy.takeDamage(damage);
        cout << "After attack, " << enemy.Name << " has " << enemy.Health << " health left.\n";
        cout << "----------------------------\n";

        userInput();

        // Generate new enemy
        if (enemy.Health <= 0) {
            cout << "You defeated the " << enemy.Name << "!\n";
            player.gainExp(20);

            // Loop for monsters (PVE)
            if (player.getHealth() > 0) { // Check if player is still alive before generating new enemy
                int enemyType = rand() % 3; // 0: Slime, 1: Skeleton, 2: Dragon
                switch (enemyType) {
                case 0: {
                    Enemy slime("Slime", 50, 10);
                    cout << "A wild " << slime.getName() << " appears!" << endl;
                    battlePVE(player, slime); // Start battle with the new enemy
                    return;
                }
                case 1: {
                    Enemy skeleton("Skeleton", 80, 15);
                    cout << "A cursed " << skeleton.getName() << " appears!" << endl;
                    battlePVE(player, skeleton); // Start battle with the new enemy
                    return;
                }
                case 2: {
                    Enemy dragon("Dragon", 120, 30);
                    cout << "A mythical " << dragon.getName() << " appears!" << endl;
                    battlePVE(player, dragon); // Start battle with the new enemy
                    return;
                }
                default:
                    cout << "No more enemies appear.\n";
                    return;
                }
            }
            return; // Stop the current loop after handling new enemy
        }

        // Enemy's turn
        pveStatus(player, enemy); // Show status

        if (enemy.Health > 0) { // Check if enemy health is below zero.
            cout << enemy.Name << "'s turn:\n";
            cout << enemy.Name << " attacks!\n";
            player.takeDamage(enemy.Damage);
            cout << "After enemy attack, Player has " << player.getHealth() << " health left.\n";
        } else {
            break;
        }
        cout << "----------------------------\n";

        userInput();
    }
    // Determine winner
    if (player.getHealth() <= 0) {
        cout << "You lost!" << endl;
    }
    else {
        cout << "You defeated the " << enemy.Name << "!" << endl;
        player.gainExp(20);  // Award experience points after defeating an enemy
    }

    userInput();
}

// PVP battle (Updated with status updates)
void battlePVP(Character& player1, Character& player2) {
    while (player1.getHealth() > 0 && player2.getHealth() > 0) {
        pvpStatus(player1, player2); // Show status

        // Player 1's turn
        cout << "Player 1's turn:\n";
        int damage1 = 0;
        if (auto knight1 = dynamic_cast<Knight*>(&player1)) {
            damage1 = knight1->action();
        }
        else if (auto wizard1 = dynamic_cast<Wizard*>(&player1)) {
            damage1 = wizard1->action();
        }
        else if (auto assassin1 = dynamic_cast<Assassin*>(&player1)) {
            damage1 = assassin1->action();
        }
        else if (auto dracula1 = dynamic_cast<Dracula*>(&player1)) {
            damage1 = dracula1->action();
        }
        player2.takeDamage(damage1);
        // Show status update
        cout << "Player 1 attacks with " << damage1 << " damage!" << endl;
        cout << "----------------------------\n";

        // Wait for user input before Player 2's turn
        cout << "Press Enter to continue...\n";
        cin.ignore(); // Clear the newline character from previous input
        cin.get();    // Wait for the user to press Enter

        if (player2.getHealth() <= 0) break; // Check if player2's health is below zero.

        pvpStatus(player1, player2); // Show status

        // Player 2's turn
        cout << "Player 2's turn:\n";
        int damage2 = 0;
        if (auto knight2 = dynamic_cast<Knight*>(&player2)) {
            damage2 = knight2->action();
        }
        else if (auto wizard2 = dynamic_cast<Wizard*>(&player2)) {
            damage2 = wizard2->action();
        }
        else if (auto assassin2 = dynamic_cast<Assassin*>(&player2)) {
            damage2 = assassin2->action();
        }
        else if (auto dracula2 = dynamic_cast<Dracula*>(&player2)) {
            damage2 = dracula2->action();
        }
        player1.takeDamage(damage2);
        // Show status update
        cout << "Player 2 attacks with " << damage2 << " damage!" << endl;
        cout << "----------------------------\n";

        // Wait for user input before the next round
        cout << "Press Enter to continue...\n";
        cin.ignore(); // Clear the newline character from previous input
        cin.get();    // Wait for the user to press Enter
    }

    pvpStatus(player1, player2); // Show status

    if (player1.getHealth() <= 0) {
        cout << "Player 2 wins!" << endl;
    }
    else {
        cout << "Player 1 wins!" << endl;
    }

    cout << "Press Enter to return to the main menu...\n";
    cin.ignore(); // Clear any leftover newline character
    cin.get();    // Wait for the user to press Enter
}

// ===== Function: Main Menu =====
void mainMenu() {
    system("cls");
    printText("Welcome to RPG4L1F3!", YELLOW);
    cout << "1. PVP (Player vs Player)" << endl;
    cout << "2. PVE (Player vs Environment)" << endl;
    cout << "3. Exit game" << endl;
    cout << "Choose your mode: ";
}

/// ===== Main Function =====
int main()
{
    srand(static_cast<unsigned int>(time(0)));
    int modeChoice;
    do {
        mainMenu(); // Display the main menu
        cin >> modeChoice;

        if (modeChoice == 1) {
            // PVP Mode
            system("cls");
            printText("PVP Mode selected.", YELLOW);
            cout << "Player 1, Choose your character:\n1. Knight\n2. Wizard\n3. Assassin\n4. Dracula\n";
            int choice1;
            cin >> choice1;
            Character* player1;
            if (choice1 == 1) {
                player1 = new Knight();
            }
            else if (choice1 == 2) {
                player1 = new Wizard();
            }
            else if (choice1 == 3) {
                player1 = new Assassin();
            }
            else {
                player1 = new Dracula();
            }
            cout << "Player 2, Choose your character:\n1. Knight\n2. Wizard\n3. Assassin\n4. Dracula\n";
            int choice2;
            cin >> choice2;
            Character* player2;
            if (choice2 == 1) {
                player2 = new Knight();
            }
            else if (choice2 == 2) {
                player2 = new Wizard();
            }
            else if (choice2 == 3) {
                player2 = new Assassin();
            }
            else {
                player2 = new Dracula();
            }
            battlePVP(*player1, *player2);
            delete player1;
            delete player2;
        }
        else if (modeChoice == 2) {
            // PVE Mode
            system("cls");
            printText("PVE Mode selected.", YELLOW);
            cout << "Choose your character:\n1. Knight\n2. Wizard\n3. Assassin\n4. Dracula\n";
            int choice;
            cin >> choice;
            Character* player;
            if (choice == 1) {
                player = new Knight();
            }
            else if (choice == 2) {
                player = new Wizard();
            }
            else if (choice == 3) {
                player = new Assassin();
            }
            else {
                player = new Dracula();
            }

            // Loop for monsters (PVE)
            Enemy slime("Slime", 50, 10);
            cout << "A wild " << slime.getName() << " appears!" << endl;
            battlePVE(*player, slime);

            delete player;
        }
        else if (modeChoice == 3) {
            cout << "Exiting the game..." << endl;
            break;  // Exit the loop
        }
        else {
            printText("Invalid choice. Please select a valid option.", RED);
            // cout << "Invalid choice. Please select a valid option." << endl;
            userInput();
        }
    } while (true);
    return 0;
}
