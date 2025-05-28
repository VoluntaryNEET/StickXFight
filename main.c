#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //for random seed each time
#include <stdbool.h>
#include <windows.h>
#include "Fights.h" //Add the functions in the main fight library
int Movelist(int weapon, char first_move[], char second_move[]);
int player_hp=3, weapon, level, Help_Screen(), Start_Screen(), Fight1(), Weapon_Selection();
int LoadGame(int *player_hp, int *level, int *weapon, int *ult_charge, char Move1[], char Move2[]) {
    FILE *file = fopen("Savefile.txt", "r"); // "r" = read mode
    if (file == NULL) {
        printf("Your save file wasn't found!\n");
        Sleep(1000);
        return 0;
    }
    // Read the data
    fscanf(file, "%d", player_hp);
    fscanf(file, "%d", level);
    fscanf(file, "%d", weapon);
    fscanf(file, "%d", ult_charge);
    fscanf(file, "%s", Move1);
    fscanf(file, "%s", Move2);
    fclose(file);
}
int Start_Screen(){
    // Title Banner
    int option;
    system("cls"); //To clear the cmd so it doesn't get cluttered
    printf("\n\n");
    printf("  ****************************************************************************************************************\n");
    printf("  *                                                                                                              *\n");
    printf("  *    ******  *******  *******   ******  *  *      *   *        *******  *******   *******   *    *  *******    *\n");
    printf("  *   *           *        *      *       * *        * *         *           *      *         *    *     *       *\n");
    printf("  *    ******     *        *      *       **          *          ******      *      *  ****   ******     *       *\n");
    printf("  *         *     *        *      *       * *        * *         *           *      *     *   *    *     *       *\n");
    printf("  *    *****      *     *******   ******  *  *      *   *        *        *******   *******   *    *     *       *\n");
    printf("  *                                                                                                              *\n");
    printf("  ****************************************************************************************************************\n");
    // Menu Options
    printf("\n");
    printf("     1. New Game\n");
    printf("       2. Load Game\n");
    printf("         3. Help\n");
    printf("           4. Quit\n\n");
    option = invalidcheck_int(1, 4, "Select an option (1-4): ");
    return option;
    }
int Help_Screen(){
    // Help Banner
    system("cls");
    printf("\n\n");
    printf("  *********************************************************\n");
    printf("  *                                                       *\n");
    printf("  *            *    *   ******  *       ******            *\n");
    printf("  *            *    *   *       *       *    *            *\n");
    printf("  *            ******   ******  *       ******            *\n");
    printf("  *            *    *   *       *       *                 *\n");
    printf("  *            *    *   ******  ******  *                 *\n");
    printf("  *                                                       *\n");
    printf("  *********************************************************\n");
    // Menu Options
    printf("\n");
    printf("GAME STRUCTURE:\n");
    printf(" - The game consists of 3 fights:\n");
    printf("     1. First Encounter\n");
    printf("     2. Mid-Level Enemy\n");
    printf("     3. Final Boss Battle\n\n");

    printf("TURN-BASED COMBAT:\n");
    printf(" - In each fight, you and your enemy take turns.\n");
    printf(" - On your turn, you can choose to attack or use an item.\n\n");

    printf("WEAPONS & MOVES:\n");
    printf(" - Choose 1 of 2 weapons:\n");
    printf("     [1] Drawnblade  (Sword)\n");
    printf("     [2] Gembrush    (Magic Staff)\n");
    printf(" - After selecting a weapon, you’ll pick 2 of 3 available moves:\n");
    printf("     • Basic Attack\n");
    printf("     • Special Skill\n");
    printf("     • Ultimate Move (requires charge buildup across fights)\n\n");

    printf("ULTIMATE MOVES:\n");
    printf(" - You must charge your ultimate through attacks to unlock it.\n");
    printf(" - Once charged, you can unleash a powerful move.\n\n");

    printf("ITEMS:\n");
    printf(" - During combat, select 'Items' to access healing potions or other tools.\n\n");

    printf("SAVING & LOADING:\n");
    printf(" - After fights, you may save your progress.\n");
    printf(" - Loading will bring you back to the exact moment you left off.\n\n");

    printf("TIPS:\n");
    printf(" - Plan your moves and manage item use carefully.\n");
    printf(" - Ultimate moves can turn the tide, so charge wisely.\n");
    printf(" - Don’t forget to save before a boss fight!\n\n");
    int temp = invalidcheck_int(0, 9999, "\nInput ONLY a number to go back to the Start screen.\n");
}
int Weapon_Selection(){
    char affirmation, Move1[100], Move2[100];
    system("cls");
    printf("\n\n");
    printf("  *******************************************************************************\n");
    printf("  *                                                                             *\n");
    printf("  *   *       *       *  ******      *      ******     ***    *    *    *****   *\n");
    printf("  *    *     * *     *   *          * *     *     *   *   *   * *  *   *        *\n");
    printf("  *     *   *   *   *    ******    *   *    ******   *     *  *  * *    ****    *\n");
    printf("  *      * *     * *     *        * * * *   *         *   *   *   **        *   *\n");
    printf("  *       *       *      ******  *       *  *          ***    *    *   *****    *\n");
    printf("  *                                                                             *\n");
    printf("  *******************************************************************************\n\n\n");
    printf("                                                      + \n");
    printf("               |                                   * +++ *\n");
    printf("               |                                    *+++*  \n");
    printf("               |    Name: Drawnblade                 ***   Name: Gembrush\n");
    printf("               |                                      * \n");
    printf("               |                                      *\n");
    printf("             --*--                                    *\n");
    printf("               |                                      *\n");
    printf("               |                                      * \n");
    weapon = invalidcheck_int(1, 2, "Select your weapon (1-2): ");
    switch (weapon){
    case 1:
        printf("You've successfully chosen the 'Drawnblade'");
        break;
    case 2:
        printf("You've successfully chosen the 'Gembrush'");
        break;
    }
    Movelist(weapon, Move1, Move2); //Function to return and save what moves to use during fights
    system("cls");
    affirmation = invalidcheck_char("\n\nReady to start your adventure? (Y/N) \n", "YN");
    if (affirmation == 'Y'){
    enemy_staff_hp = 2; //Initializing enemyhealth
    enemy_archer_hp = 3;
    potions = 3;
    enemy_boss_hp = 7;
    player_hp = 3;
    ult_charge = 0;
    if (weapon == 1){ //Move to first fight
        Fight1_Sword_Idle(&player_hp, Move1, Move2);
    }
    else if (weapon == 2){
        Fight1_Mage_Idle(&player_hp, Move1, Move2);
    }
    }
    else if (affirmation == 'N'){
        //Empty means it will go back to Start screen in main loop due to true while loop
    }
}
int Movelist(int weapon, char first_move[], char second_move[]) {
    int first_choice, second_choice;
    system("cls");
    printf("\n\n");
    printf("  ********************************************************************************\n");
    printf("  *                                                                              *\n");
    printf("  *    *         *    ***   *       *  ******  *       *******   *****  *******  *\n");
    printf("  *    * *     * *   *   *   *     *   *       *          *     *          *     *\n");
    printf("  *    *  *   *  *  *     *   *   *    ******  *          *      ****      *     *\n");
    printf("  *    *   * *   *   *   *     * *     *       *          *          *     *     *\n");
    printf("  *    *    *    *    ***       *      ******  ******  *******  *****      *     *\n");
    printf("  *                                                                              *\n");
    printf("  ********************************************************************************\n\n\n");
    printf("Complete Move list: (You can only select two moves for a journey)\n");
    // Weapon moves arrays
    char *sword_moves[] = {"Sprint", "HookCut", "Ragnarok"};
    char *staff_moves[] = {"Firestorm", "Thunderspell", "Globaleclipse"};
    char **temp; //Double pointer since need to make it one of the two above arrays depending on weapon
    if (weapon == 1) {
        temp = sword_moves;
    } else if (weapon == 2) {
        temp = staff_moves;
    }
    printf("     1. %s\n", temp[0]);
    printf("       2. %s\n", temp[1]);
    printf("         3. %s (Ultimate Attack)\n\n", temp[2]);
    first_choice = invalidcheck_int(1, 3, "Select the first move to equip (1-3): ");
    second_choice = invalidcheck_int(1, 3, "Select the second move to equip (1-3): ");
    strcpy(first_move, temp[first_choice - 1]);
    strcpy(second_move, temp[second_choice - 1]);
}
int main() {
    srand(time(0)); //Randomize seed so same random values dont appear every run
    player_hp = 3;
    level = 1;
    char Move1[100], Move2[100];
    while (true){       // Loop in order to keep game running until quit
    switch (Start_Screen()){    // Menu Selection using Switch case
    case 1:
        Weapon_Selection();
        break;
    case 2:
        LoadGame(&player_hp, &level, &weapon, &ult_charge, Move1, Move2); //Read values from save file to decide which fight to run
        if (level == 1 && weapon == 1){
            Fight1_Sword_Idle(&player_hp, Move1, Move2);}
        else if (level == 1 && weapon == 2){
            Fight1_Mage_Idle(&player_hp, Move1, Move2);}
        else if (level == 11){
            Fight2_Sword_Idle(&player_hp, Move1, Move2);}
        else if (level == 12){
            Fight3_Sword_Idle(&player_hp, Move1, Move2);}
        else if (level == 21){
            Fight2_Mage_Idle(&player_hp, Move1, Move2);}
        else if (level == 22){
            Fight3_Mage_Idle(&player_hp, Move1, Move2);}
        break;
    case 3:
        Help_Screen();
        break;
    case 4:
        return 0; // exit game
    }
}
}
