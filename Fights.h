int enemy_staff_hp = 2, enemy_archer_hp = 3, enemy_boss_hp = 7, item_menu = 0, potions = 3, enemy_turn = 0, ult_charge;
char Move1_temp[100], Move2_temp[100];
int (*tempfunc)(); //this makes sense for the item function
int Sword_Staff_Throw(int *player_hp), Sword_Archer_Shot(int *player_hp), Mage_Staff_Throw(int *player_hp), Mage_Archer_Shot(int *player_hp), Boss_Mage_Attack1(int *player_hp), Boss_Mage_Attack2(int *player_hp), Boss_Sword_Attack1(int *player_hp), Boss_Sword_Attack2(int *player_hp);
int Fight1_Sword_Idle(int *player_hp, char Move1[], char Move2[]),  Fight1_Sword_Sprint(int *player_hp), Fight1_Sword_Hook(int *player_hp), Fight1_Sword_Ult(int *player_hp);
int Fight1_Mage_Idle(int *player_hp, char Move1[], char Move2[]), Fight1_Mage_Firestorm(int *player_hp), Fight1_Mage_Thunderspell(int *player_hp), Fight1_Mage_Ult(int *player_hp);
int Fight2_Sword_Idle(int *player_hp, char Move1[], char Move2[]), Fight2_Sword_Sprint(int *player_hp), Fight2_Hook(int *player_hp), Fight2_Sword_Ult(int *player_hp);
int Fight2_Mage_Idle(int *player_hp, char Move1[], char Move2[]), Fight2_Mage_Firestorm(int *player_hp), Fight2_Mage_Thunderspell(int *player_hp), Fight2_Mage_Ult(int *player_hp);
int Fight3_Sword_Idle(int *player_hp, char Move1[], char Move2[]), Fight3_Sword_Hook(int *player_hp), Fight3_Sword_Ult(int *player_hp), Fight3_Sword_Sprint(int *player_hp);
int Fight3_Mage_Idle(int *player_hp, char Move1[], char Move2[]), Fight3_Mage_Firestorm(int *player_hp), Fight3_Mage_Thunderspell(int *player_hp), Fight3_Mage_Ult(int *player_hp);
int Fight_VictoryScreen(int level, int *player_hp), Fight_DeathScreen(int *player_hp), GameFinished();
int invalidcheck_int(int min1, int max1, const char *originalsen) { //both getvalid int and char are just standard ways to ensure no wrong user inputs are accepted during selection
    int value;
    char c;
    while (1) {
        printf("%s", originalsen);
        if (scanf("%d%c", &value, &c) != 2 || c != '\n') { //Number checker. %d%c read both (hence 2) and %c should be /n. If not, error is prompted.
            printf("Invalid input! Please enter a number between %d and %d.\n", min1, max1);
            while ((c = getchar()) != '\n' && c != EOF); // Keeps reading values into c until input buffer gets cleared
        } else if (value < min1 || value > max1) {
            printf("Please enter a valid number between %d and %d.\n", min1, max1);
        } else {
            return value;
        }
    }
}
char invalidcheck_char(const char *originalsen, const char valid_options[]) {
    char input;
    char c;
    while (1) {
        printf("%s", originalsen);
        if (scanf(" %c%c", &input, &c) != 2 || c != '\n') {
            printf("Invalid input! Please enter a valid character.\n");
            while ((c = getchar()) != '\n' && c != EOF); // Keeps reading values into c until input buffer gets cleared, then moves to next statement
        } else {
            // Convert to uppercase to people can type lowercase version too
            input = toupper(input);
            for (int i = 0; valid_options[i] != '\0'; i++) {//validoptions array contains what characters user can enter
                if (input == toupper(valid_options[i])) {
                    return input;
                }
            }
            printf("Please enter one of the valid options: %s\n", valid_options);
        }
    }
}

void HP_Bar(int player_hp, int enemy_hp){
    printf("Player HP: ");
    for (int i = 0; i < player_hp; i++) {
        printf("<3 ");
    }
    printf("\n");
    printf("Enemy HP: ");
    for (int i = 0; i < enemy_hp; i++) {
        printf("<3 ");
    }
}
int Move_List(char Move1[], char Move2[]){ //So I can print the move selection more easily for every fight
    int action;
    printf("Move List: \n");
    printf("1. ");
    fputs(Move1, stdout);
    if (strcmp(Move1, "Globaleclipse") == 0 || strcmp(Move1, "Ragnarok") == 0){
        printf(" (Ult charge at: %d%%)", ult_charge);
    }
    printf("                             2. ");
    fputs(Move2, stdout);
    if (strcmp(Move2, "Globaleclipse") == 0 || strcmp(Move2, "Ragnarok") == 0){
        printf(" (Ult charge at: %d%%)", ult_charge);
    }
    printf("\n3. Items");
    action = invalidcheck_int(1, 3, "\n\nWhat's your move?\n");
    return action;
}
int Items(int *player_hp,int *potion, char Move1[], char Move2[]) //Item menu during game
{
    int choose;
    printf("\nYour items include:\n");
    printf("1. HP potion (x%d)\n",*potion);
    printf("2. Exit Item menu");
    choose = invalidcheck_int(1, 2, "\n\nSelect an item: ");
    if (choose==1){
        if(*potion>0){
            *player_hp=*player_hp+3;
            if(*player_hp>5){
                *player_hp=5;
            }
            (*potion)--;
            printf("\nPotion successfully consumed!");
            }
    else {
        printf("\nYOU DONT HAVE ANY HP POTIONS LEFT!!");
        }
    if (choose == 2){
        return 0; //exit back
    }
    }
    Sleep(1000);
    item_menu = 0;
    tempfunc(player_hp, Move1, Move2);
}
int SaveGame(int player_hp, int level, int weapon, int ult_charge, char Move1[], char Move2[]){
    FILE *file = fopen("Savefile.txt", "w");  // "w" = write mode

    if (file == NULL) {
        printf("Error opening save file!\n");
        return 0;
    }
    // Write the data
    fprintf(file, "%d\n", player_hp);
    fprintf(file, "%d\n", level);
    fprintf(file, "%d\n", weapon);
    fprintf(file, "%d\n", ult_charge);
    fprintf(file, "%s\n", Move1);
    fprintf(file, "%s\n", Move2);
    fclose(file); // Always close the file
}
int Fight1_Sword_Idle(int *player_hp, char Move1[], char Move2[]) {
    int action;
    if (*player_hp <= 0){ //If player dies
        Fight_DeathScreen(player_hp);
        return 0;
    }
    if (enemy_staff_hp <=0){ //Move to victory screen when enemy dies
        Fight_VictoryScreen(11, player_hp);
        return 0;
    }
    if (enemy_turn == 1){ //function for enemy attack, alternating every turn
        enemy_turn = 0;
        Sword_Staff_Throw(player_hp);
    }
    else{
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    //EDITING ZONE
    printf("    **      /                                                        ^^   | \n");
    printf("   *  *    /                                                        ^  ^  | \n");
    printf("    **    /                                                          ^^   |\n");
    printf("   * *   /                                                           ^^   |    \n");
    printf("  * * * /                                                           ^ ^  ^|\n");
    printf(" *  *  ^ -                                                         ^  ^   |\n");
    printf("   * */                                                              ^ ^  | \n");
    printf("  *   *                                                             ^   ^ | \n");
    printf(" *     *                                                           ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    if (item_menu == 0){
    action = Move_List(Move1, Move2);
    strcpy(Move1_temp, Move1);
    strcpy(Move2_temp, Move2);
    if (action == 1){ //frame animation plays based on chosen move
        enemy_turn = 1; //resetting enemy turn after this
        if (strcmp(Move1_temp, "Sprint") == 0){
        ult_charge += 25; //adding ultimate charge after every move
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Sword_Sprint(player_hp);}
        if (strcmp(Move1_temp, "HookCut") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Sword_Hook(player_hp);}
        if (strcmp(Move1_temp, "Ragnarok") == 0){
            if (ult_charge >= 100){
                Fight1_Sword_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 2){ //copy pasted code but for second move
        enemy_turn = 1;
        if (strcmp(Move2_temp, "Sprint") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Sword_Sprint(player_hp);}
        if (strcmp(Move2_temp, "HookCut") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Sword_Hook(player_hp);}
        if (strcmp(Move2_temp, "Ragnarok") == 0){
            if (ult_charge >= 100){
                Fight1_Sword_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 3){ // displays fight again, but now item menu also active
        item_menu = 1;
        Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp);
    }
    return action;
    }
    else if (item_menu == 1){
        tempfunc = Fight1_Sword_Idle;
        Items(player_hp, &potions, Move1_temp, Move2_temp);
    }
}
}
int Fight1_Sword_Sprint(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n");
    printf("                                                                           come on then           \n");
    printf("                                          **                                    ^^    | \n");
    printf("                           |             *  *                                  ^  ^   | \n");
    printf("                   --------|--^ * *  **  **                                     ^^    |\n");
    printf("                           |         *   **                                 ^  ^ ^    |\n");
    printf("                                   ** **    **                                ^  ^ ^ ^|\n");
    printf("                                  **  **                                         ^    |\n");
    printf("                               **      **                                        ^    |\n");
    printf("                             **        **                                       ^ ^   | \n");
    printf("                           *         **                                        ^   ^  | \n");
    printf("                                    *                                         ^     ^   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000); //Allow the previous print statements to be seen by player
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n");
    printf("                                                                 ...ouch                    \n");
    printf("                                                           **     / ^^   | \n");
    printf("                                                          *  *   / ^  ^  | \n");
    printf("                                                           **   /   ^^   |\n");
    printf("                                                         * *  \\/   ^ ^   |\n");
    printf("                                                       *   * */\\  ^   ^ ^|\n");
    printf("                                                      *    * /   ^   ^   |\n");
    printf("                                                     *     *    ^    ^   |\n");
    printf("                                                          * *      ^  ^  | \n");
    printf("                                                         *   *    ^    ^ | \n");
    printf("                                                       *     *   ^      ^   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_staff_hp = enemy_staff_hp -1;
    Sleep(1000);
    Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp); //Go back to main function for fight
}
int Fight1_Sword_Hook(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("                                                              what is tha-  \n");
    printf("     **                                                             ^^    | \n");
    printf("    *  *                                                           ^  ^   | \n");
    printf("     **                                                             ^^    |\n");
    printf("    * *                                                          }  ^ ^   |\n");
    printf("  *  * *(---------------------------------------------------------}^ ^ ^ ^|\n");
    printf(" *   *                                                           }^  ^    |\n");
    printf("*    **                                                          ^   ^    |\n");
    printf("    *  *                                                            ^ ^   |  \n");
    printf("   *    *                                                          ^   ^  |  \n");
    printf("  *      *                                                        ^     ^     \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("               ...oh no \n");
    printf("     **          ^^    | \n");
    printf("    *:)*        ^  ^   | \n");
    printf("     **          ^^    |\n");
    printf("    * *          ^ ^   |\n");
    printf("  *  *  *       ^ ^ ^ ^|\n");
    printf(" *   *   *     ^  ^    |\n");
    printf("*    **       ^   ^    |\n");
    printf("    *  *         ^ ^   | \n");
    printf("   *    *       ^   ^  |  \n");
    printf("  *      *     ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                       ^^                       \n");
    printf("                      ^  ^                  /  \n");
    printf("                      ^^^                  /  \n");
    printf("                    ^ ^ ^                 /  \n");
    printf("                 ^  ^  ^  ^              / \n");
    printf("         !POW! ^    ^ ^   ^             / \n");
    printf("      **   *       ^  ^   ^            /   \n");
    printf("     *  *  *      ^   ^               /  \n");
    printf("      **  *      ^    ^              /  \n");
    printf("    * * *                           / \n");
    printf("  *   *                            / \n");
    printf(" *    *  \n");
    printf("*    **   \n");
    printf("    * *   \n");
    printf("   *   *  \n");
    printf("  *   *         \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_staff_hp = enemy_staff_hp -1;
    Sleep(1000);
    Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight1_Sword_Ult(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                 \n");
    printf("    -*-       **       -*-                                                              \n");
    printf(" -*-   -*-   *  *   -*-   -*-                                                           \n");
    printf(" -*-    -*-   **   -*-    -*-                                                              \n");
    printf("   -*-       * *        -*-                                                            \n");
    printf("    -*-   *   *  *     -*-                                                   ^^        \n");
    printf("     -*- *   *    *   -*-                                                   ^  ^  |  \n");
    printf("      _/_   *  *  _\\_                                                        ^^   |   \n");
    printf("     /  /  *    * \\  \\                                                       ^^   |  \n");
    printf("    /  /  *    *   \\  \\                                                     ^ ^  ^|    \n");
    printf("   /  / *     *     \\  \\                                                   ^  ^   |  \n");
    printf("  /  /               \\  \\                                                    ^ ^  |   \n");
    printf(" /  /                 \\  \\                                                  ^   ^ |   \n");
    printf(" \\ /                   \\ /                                                 ^     ^    \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("    -*-       **      -*-                                                              \n");
    printf(" -*-   -*-   *  *  -*-   -*-                                               ^^           \n");
    printf(" -*-    -*-   **  -*-    -*-                                              ^  ^  |          \n");
    printf("   -*-       * *       -*-                                                 ^^   |       \n");
    printf("    -*-   *   *   *   -*-                                                  ^^   |      \n");
    printf("     -*- *    *    * -*-                                                  ^ ^  ^|        \n");
    printf("       _|_   * *   _|_                                                   ^  ^   |        \n");
    printf("       | |  *   *  | |                                                     ^ ^  |          \n");
    printf("     \\ | | *     * | | /                                                  ^   ^ |          \n");
    printf("      \\| | *     * | |/                                                  ^     ^         \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                               ^       ^^             \n");
    printf("    -*-       **      -*-                                        ^   ^   ^        \n");
    printf(" -*-   -*-   *  *  -*-   -*-                                        ^ ^^           \n");
    printf(" -*-    -*-   **  -*-    -*-                                 ^        ^  ^        /   \n");
    printf("   -*-       * *       -*-                                      ^    ^     ^     /     \n");
    printf("    -*-   *   *   *   -*-                                         ^      ^      /    \n");
    printf("     -*- *    *    * -*-                              __    ^  ^               /     \n");
    printf("       _|_   * *   _|_                            __ /  / /                   /      \n");
    printf("       | |  *   *  | |                          _/    /  /                   /       \n");
    printf("     \\ | | *     * | | /                   __/     /    /                   /       \n");
    printf("      \\| | *     * | |/                   /      /     /                             \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_staff_hp = enemy_staff_hp -3;
    ult_charge = 0; // reset ult charge back to 0
    Sleep(1000);
    Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Sword_Staff_Throw(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n\n\n");
    printf("                                                                 TAKE THIS!       \n");
    printf("      **       /                                                     ^^ \\        \n");
    printf("     *  *     /                                                ^    ^  ^ \\       \n");
    printf("      **     /                                                   ^ ^ ^^   \\       \n");
    printf("    * * *   /                                                        ^ ^^^ \\     \n");
    printf("   *  *  * /                                                         ^      \\      \n");
    printf("  *   *   ^ -                                                       ^        \\    \n");
    printf("     * * /                                                         ^ ^        \\ \n");
    printf("    *   *                                                         ^    ^        \n");
    printf("   *     *                                                        ^       ^       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used STAFFTHROW");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n\n\n");
    printf("                                                           \n");
    printf("      **       /                                                     ^^         \n");
    printf("     *  *     /                                                     ^  ^        \n");
    printf("      **     /                                                       ^^         \n");
    printf("    * * *   /          _______________                         ^  ^   ^  ^      \n");
    printf("   *  *  * /                                                           ^    ^   \n");
    printf("  *   *   ^ -                                                          ^ ^     ^\n");
    printf("     * * /                                                            ^   ^  ^  \n");
    printf("    *   *                                                             ^     ^   \n");
    printf("   *     *                                                            ^         \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 1;
    Sleep(1000);
    Fight1_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight2_Sword_Idle(int *player_hp, char Move1[], char Move2[]) {
    int action;
    if (*player_hp <= 0){
        Fight_DeathScreen(player_hp);
        return 0;
    }
    if (enemy_archer_hp <=0){
        Fight_VictoryScreen(12, player_hp);
        return 0;
    }
    if (enemy_turn == 1){
        enemy_turn = 0;
        Sword_Archer_Shot(player_hp);
    }
    else{
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n");
    //EDITING ZONE
    printf("                                                                            \n");
    printf("                                                                               \n");
    printf("                                                                                 \n");
    printf("    **      /                                                         *     ^^    \n");
    printf("   *  *    /                                                          | *  ^  ^   \n");
    printf("    **    /                                                           |  *  ^^    \n");
    printf("   * *   /                                                            |^^^^^  ^^  \n");
    printf("  * * * /                                                             |  *   ^    \n");
    printf(" *  *  ^ -                                                            | *   ^     \n");
    printf("   * */                                                               *   ^ ^     \n");
    printf("  *   *                                                                  ^   ^    \n");
    printf(" *     *                                                                ^     ^    \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    if (item_menu == 0){
    action = Move_List(Move1, Move2);
    strcpy(Move1_temp, Move1);
    strcpy(Move2_temp, Move2);
    if (action == 1){
        enemy_turn = 1;
        if (strcmp(Move1_temp, "Sprint") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Sword_Sprint(player_hp);}
        if (strcmp(Move1_temp, "HookCut") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Hook(player_hp);}
        if (strcmp(Move1_temp, "Ragnarok") == 0){
            if (ult_charge >= 100){
                Fight2_Sword_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 2){
        enemy_turn = 1;
        if (strcmp(Move2_temp, "Sprint") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Sword_Sprint(player_hp);}
        if (strcmp(Move2_temp, "HookCut") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Hook(player_hp);}
        if (strcmp(Move2_temp, "Ragnarok") == 0){
            if (ult_charge >= 100){
                Fight2_Sword_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 3){
        item_menu = 1;
        Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
    }
    return action;
    }
    else if (item_menu == 1){
        tempfunc = Fight2_Sword_Idle;
        Items(player_hp, &potions, Move1_temp, Move2_temp);
    }
    }
}
int Fight2_Sword_Sprint(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n");
    printf("                                                                           come on then           \n");
    printf("                                          **                                         \n");
    printf("                           |             *  *                               *     ^^    \n");
    printf("                   --------|--^ * *  **  **                                 | *  ^  ^    \n");
    printf("                           |         *   **                                 |  *  ^^    \n");
    printf("                                   ** **    **                              |^^^^^  ^^\n");
    printf("                                  **  **                                    |  *   ^    \n");
    printf("                               **      **                                   | *   ^    \n");
    printf("                             **        **                                   *   ^ ^    \n");
    printf("                           *         **                                        ^   ^   \n");
    printf("                                    *                                         ^     ^   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n");
    printf("                                                                 ...ouch \n");
    printf("                                                           **     / ^^ \n");
    printf("                                                          *  *   / ^  ^\n");
    printf("                                                           **   /   ^^ \n");
    printf("                                                         * *  \\/   ^ ^ \n");
    printf("                                                       *   * */\\ ^     ^     * \n");
    printf("                                                      *    * /  ^    ^   ^  / * \n");
    printf("                                                     *     *         ^    ^/  *  \n");
    printf("                                                          * *      ^  ^   /  * \n");
    printf("                                                         *   *    ^    ^ * * \n");
    printf("                                                       *     *   ^      ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_archer_hp = enemy_archer_hp -1;
    Sleep(1000);
    Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight2_Hook(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("     **                                                           ^^ \n");
    printf("    *  *                                                         ^  ^   *\n");
    printf("     **                                                           ^^    | *\n");
    printf("    * *                                                        }  ^ ^   |  *\n");
    printf("  *  * *(-------------------------------------------------------}^ ^ ^^^|  *\n");
    printf(" *   *                                                         }^  ^    |  *\n");
    printf("*    **                                                        ^   ^    | *\n");
    printf("    *  *                                                          ^ ^   *\n");
    printf("   *    *                                                        ^   ^    \n");
    printf("  *      *                                                      ^     ^\n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("               ...oh no \n");
    printf("     **          ^^  \n");
    printf("    *:)*        ^  ^   *  \n");
    printf("     **          ^^    | * \n");
    printf("    * *          ^ ^   |  *\n");
    printf("  *  *  *       ^ ^ ^^^|  *\n");
    printf(" *   *   *     ^  ^    |  *\n");
    printf("*    **       ^   ^    | * \n");
    printf("    *  *         ^ ^   *\n");
    printf("   *    *       ^   ^\n");
    printf("  *      *     ^     ^\n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                       ^^                               \n");
    printf("                      ^  ^                  *            \n");
    printf("                      ^^^                  / *           \n");
    printf("                    ^ ^ ^                 /  *          \n");
    printf("                 ^  ^  ^  ^              /   *           \n");
    printf("         !POW! ^    ^ ^   ^             /   *            \n");
    printf("      **   *       ^  ^   ^            /   *             \n");
    printf("     *  *  *      ^   ^               *  *              \n");
    printf("      **  *      ^    ^                                 \n");
    printf("    * * *                                               \n");
    printf("  *   *                                                 \n");
    printf(" *    *  \n");
    printf("*    **   \n");
    printf("    * *   \n");
    printf("   *   *  \n");
    printf("  *   *         \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_archer_hp = enemy_archer_hp -1;
    Sleep(1000);
    Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Sword_Archer_Shot(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
   printf("\n------------------------------------------------------------------------------------\n\n\n\n\n");
    printf("                                                                               \n");
    printf("                                                                                 \n");
    printf("    **      /                                                           *  ^^    \n");
    printf("   *  *    /                                                           * \\^  ^   \n");
    printf("    **    /                                                        <- *   \\^^    \n");
    printf("   * *   /                                                         <-*^^^^^^  ^^  \n");
    printf("  * * * /                                                          <- *   /  ^    \n");
    printf(" *  *  ^ -                                                             * /  ^     \n");
    printf("   * */                                                                 * ^ ^     \n");
    printf("  *   *                                                                  ^   ^    \n");
    printf(" *     *                                                                ^     ^    \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used TRIFECTA");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n");
    printf("                                                                            \n");
    printf("                                                                               \n");
    printf("                                                                                 \n");
    printf("    **      /                                                              \n");
    printf("   *  *    /                                                     *       ^^   \n");
    printf("    **    /                                                      * \\    ^  ^  \n");
    printf("   * *   /              <-----------                             *  \\    ^^  ^\n");
    printf("  * * * /               <-----------                              *  \\^^^  ^^ \n");
    printf(" *  *  ^ -              <-----------                                * *    ^   \n");
    printf("   * */                                                                 ^ ^    \n");
    printf("  *   *                                                               ^   ^    \n");
    printf(" *     *                                                              ^   ^^   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 1;
    Sleep(1000);
    Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight2_Sword_Ult(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                    TRANSFORM!!!!                                                             \n");
    printf("    -*-       **       -*-                                                              \n");
    printf(" -*-   -*-   *  *   -*-   -*-                                                           \n");
    printf(" -*-    -*-   **   -*-    -*-                                                              \n");
    printf("   -*-       * *        -*-                                                            \n");
    printf("    -*-   *   *  *     -*-                                               *     ^^    \n");
    printf("     -*- *   *    *   -*-                                                | *  ^  ^   \n");
    printf("      _/_   *  *  _\\_                                                    |  *  ^^    \n");
    printf("     /  /  *    * \\  \\                                                   |^^^^^  ^^  \n");
    printf("    /  /  *    *   \\  \\                                                  |  *   ^    \n");
    printf("   /  / *     *     \\  \\                                                 | *   ^     \n");
    printf("  /  /               \\  \\                                                *   ^ ^     \n");
    printf(" /  /                 \\  \\                                                  ^   ^    \n");
    printf(" \\ /                   \\ /                                                 ^     ^    \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("    -*-       **      -*-                                                        \n");
    printf(" -*-   -*-   *  *  -*-   -*-                                           *     ^^   \n");
    printf(" -*-    -*-   **  -*-    -*-                                           | *  ^  ^    \n");
    printf("   -*-       * *       -*-                                             |  *  ^^     \n");
    printf("    -*-   *   *   *   -*-                                              |^^^^^  ^^      \n");
    printf("     -*- *    *    * -*-                                               |  *   ^       \n");
    printf("       _|_   * *   _|_                                                 | *   ^       \n");
    printf("       | |  *   *  | |                                                 *   ^ ^          \n");
    printf("     \\ | | *     * | | /                                                 ^   ^         \n");
    printf("      \\| | *     * | |/                                                 ^     ^      \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                                                 \n");
    printf("                                                               ^       ^^             \n");
    printf("    -*-       **      -*-                                        ^   ^   ^             \n");
    printf(" -*-   -*-   *  *  -*-   -*-                                        ^ ^^               \n");
    printf(" -*-    -*-   **  -*-    -*-                                 ^        ^  ^        *   \n");
    printf("   -*-       * *       -*-                                      ^    ^     ^     / *     \n");
    printf("    -*-   *   *   *   -*-                                         ^      ^      /  *  \n");
    printf("     -*- *    *    * -*-                              __    ^  ^               /   *      \n");
    printf("       _|_   * *   _|_                            __ /  / /                   /   *   \n");
    printf("       | |  *   *  | |                          _/    /  /                   /   *     \n");
    printf("     \\ | | *     * | | /                   __/     /    /                  *  *        \n");
    printf("      \\| | *     * | |/                   /      /     /                             \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_archer_hp = enemy_archer_hp -3;
    ult_charge = 0;
    Sleep(1000);
    Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight1_Mage_Idle(int *player_hp, char Move1[], char Move2[]) {
    int action;
    if (*player_hp <= 0){
        Fight_DeathScreen(player_hp);
        return 0;
    }
    if (enemy_staff_hp <=0){
        Fight_VictoryScreen(21, player_hp);
        return 0;
    }
    if (enemy_turn == 1){
        enemy_turn = 0;
        Mage_Staff_Throw(player_hp);
    }
    else{
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    //EDITING ZONE
    printf("    **    +                                                          ^^   | \n");
    printf("   *  * *+++*                                                       ^  ^  | \n");
    printf("    **   ***                                                         ^^   |\n");
    printf("   * *    *                                                          ^^   |    \n");
    printf("  * *  *  ^                                                         ^ ^  ^|\n");
    printf(" *  *     *                                                        ^  ^   |\n");
    printf("   * *    *                                                          ^ ^  | \n");
    printf("  *   *   *                                                         ^   ^ | \n");
    printf(" *     *                                                           ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    if (item_menu == 0){
    action = Move_List(Move1, Move2);
    strcpy(Move1_temp, Move1);
    strcpy(Move2_temp, Move2);
    if (action == 1){
        enemy_turn = 1;
        if (strcmp(Move1_temp, "Firestorm") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Mage_Firestorm(player_hp);}
        if (strcmp(Move1_temp, "Thunderspell") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Mage_Thunderspell(player_hp);}
        if (strcmp(Move1_temp, "Globaleclipse") == 0){
            if (ult_charge >= 100){
                Fight1_Mage_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 2){
        enemy_turn = 1;
        if (strcmp(Move2_temp, "Firestorm") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Mage_Firestorm(player_hp);}
        if (strcmp(Move2_temp, "Thunderspell") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight1_Mage_Thunderspell(player_hp);}
        if (strcmp(Move2_temp, "Globaleclipse") == 0){
            if (ult_charge >= 100){
                Fight1_Mage_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 3){
        item_menu = 1;
        Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
    }
    return action;
    }
    else if (item_menu == 1){
        tempfunc = Fight1_Mage_Idle;
        Items(player_hp, &potions, Move1_temp, Move2_temp);
    }
    }
}
int Fight1_Mage_Firestorm(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                   ++\n");
    printf("                                  +++\n");
    printf("         +   I CAST FIRESTORM!     +   \n");
    printf("       *+++*                                  +++\n");
    printf("        ***                +++               +++++              oh yea no that's-\n");
    printf("    **   *                +++++               +++                     ^^   | \n");
    printf("   *  *  ^                 +++                                       ^  ^  | \n");
    printf("    ** * *                                                            ^^   |\n");
    printf("   * *   *                                                            ^^   |    \n");
    printf("  *  *   *                                                           ^ ^  ^|\n");
    printf(" *   *                                                              ^  ^   |\n");
    printf("    * *                                                               ^ ^  | \n");
    printf("   *   *                                                             ^   ^ | \n");
    printf(" *     *                                                            ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                                                  \\           \n");
    printf("                                                                \\ +++          \n");
    printf("         +                                                     \\ +++++             \n");
    printf("       *+++*                                                      +++  \\  \\            \n");
    printf("        ***                                                           \\ +++ \n");
    printf("    **   *                                                        \\    +++++      AAAAHHHHH!!!\n");
    printf("   *  *  ^                                                      \\+++ ^  +++  ^^ \n");
    printf("    ** * *                                                     \\+++++  ^   ^   ^ \n");
    printf("   * *   *                                                       +++       ^ ^^ \n");
    printf("  *  *   *                                                          ^        ^  ^ \n");
    printf(" *   *                                                                 ^    ^     ^ \n");
    printf("    * *                                                                  ^      ^  \n");
    printf("   *   *                                                           ^  ^ \n");
    printf(" *     *      \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_staff_hp = enemy_staff_hp -1;
    Sleep(1000);
    Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight1_Mage_Thunderspell(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                             *   \n");
    printf("                                           * * ***   \n");
    printf("         +   I CAST THUNDERSPELL!     * *   *     **  \n");
    printf("       *+++*                         *   *        ** \n");
    printf("        ***                         *  * ** ***** **  \n");
    printf("    **   *                                 /   /                      ^^   | \n");
    printf("   *  *  ^                                /   /                      ^  ^  | \n");
    printf("    ** * *                                \\   /                       ^^   | \n");
    printf("   * *   *                                |  |                        ^^   |\n");
    printf("  *  *   *                                | /                        ^ ^  ^|  \n");
    printf(" *   *                                    |/                        ^  ^   |   \n");
    printf("    * *                                                               ^ ^  |  \n");
    printf("   *   *                                                             ^   ^ |   \n");
    printf(" *     *                                                            ^     ^   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------");
    printf("                                                                                                          *\n");
    printf("                                                                   * * *** \n");
    printf("                                                               * *   *     ** \n");
    printf("                                                                * * ** ***** *** * \n");
    printf("         +                                                        /|_  /|_  /|_   \n");
    printf("       *+++*                                                     /_ / /_ / /_ /  \n");
    printf("        ***                                                       |/   |/   |/   \n");
    printf("    **   *                                            AAAAAHHHHH!!!!  ^^               /  \n");
    printf("   *  *  ^                                                         ^ ^  ^  ^          /  \n");
    printf("    ** * *                                                          ^ ^^  ^          / \n");
    printf("   * *   *                                                            ^^            /  \n");
    printf("   * *   *                                                            ^^           /  \n");
    printf("  *  *   *                                                             ^          /  \n");
    printf(" *   *                                                                 ^         / \n");
    printf("    * *                                                               ^ ^       / \n");
    printf("   *   *                                                             ^   ^     /  \n");
    printf(" *     *                                                            ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_staff_hp = enemy_staff_hp -1;
    Sleep(1000);
    Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight1_Mage_Ult(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                               |                                        \n");
    printf("                /       |         *                              \\                   \n");
    printf("                                    *      |       |                                  \n");
    printf("                          ---  +  *  *                                                  \n");
    printf("                      ---    +    + *   **********************           \\              \n");
    printf("                          ---  +  *  *                                                  \n");
    printf("             /                      *                               \\                     \n");
    printf("                     /            *           **              \\                              \n");
    printf("                                       *    *   *   *                      /              \n");
    printf("            \\                            *   **   *                                         \n");
    printf("                   \\             |          * *        |          /                      \n");
    printf("                                             *                                             \n");
    printf("                      \\        |             *           |      /                        \n");
    printf("                                           *   *                                        \n");
    printf("                                          *     *                                      \n");
    printf("                                    \\     *     *    /                                   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                         ***                              \n");
    printf("                                                                              \n");
    printf("                                      *       *                               \n");
    printf("                               _                     _                       \n");
    printf("                              / \\        ***        / \\                       \n");
    printf("                                  *  *   * *   *  *                      \n");
    printf("                                          *                               \n");
    printf("                                          *                               \n");
    printf("                                  *       *       *                     \n");
    printf("                          *       ---------------         *                \n");
    printf("                    *                                           *       \n");
    printf("               *      ----------------------------------------       *   \n");
    printf("            *                                                           *    \n");
    printf("         *    ------------------------------------------------------       *     \n");
    printf("       *                                                                     *   \n");
    printf("      *                                                                       *  \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                      ************       \n");
    printf("                                 *****            *****     \n");
    printf("                               ***   --------------   ***   \n");
    printf("                             **                          **  \n");
    printf("                           **   -----------------------    ** \n");
    printf("                          **                                ** \n");
    printf("                          **                                **\n");
    printf("                          ** ------------------------------ **\n");
    printf("                          **                                **\n");
    printf("                          **                                **\n");
    printf("                          **                                **\n");
    printf("                           **   -----------------------    **\n");
    printf("                            **                            ** \n");
    printf("                              **                        **  \n");
    printf("                               ***   --------------   ***   \n");
    printf("                                  *****          *****     \n");
    printf("                                      ************       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                               .     *       .       *    .       \n");
    printf("                            .    *    @    .     *     @     .    \n");
    printf("                          *   .   *  ###  *   BOOM!   ###   *  .  \n");
    printf("                             *  ++++++++   @@@@@@@@   ++++*       \n");
    printf("                          *  @@@###########@@@@@@@########@@@ *   \n");
    printf("                            **##@@@######@@@#######@@@######**    \n");
    printf("                         *  ++++@@@######@@@+++++++@@@######++  * \n");
    printf("                           @@@###############@@@@@@############@  \n");
    printf("                            *   @@@######@@@   *   @@@######@@@   \n");
    printf("                         *  @@+++########+++@@     @@+++####+++@@ \n");
    printf("                            *   .  ######  .   *     ######  . *  \n");
    printf("                         .    *     @@@     *    .    @@@    *    \n");
    printf("                            .   *   .   .     *  .    .   *    .  \n");
    printf("                         .     *     .   .    *   .    .    *     \n");
    printf("                            *     .     *   .    *    .   *    .  \n");
    printf("                           .   *      .   *    .    *     .       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_staff_hp = enemy_staff_hp -99;
    ult_charge = 0;
    Sleep(1000);
    Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Mage_Staff_Throw(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n\n\n");
    printf("                                                                 TAKE THIS!       \n");
    printf("      **    +                                                        ^^ \\        \n");
    printf("     *  * *+++*                                                ^    ^  ^ \\       \n");
    printf("      **   ***                                                   ^ ^ ^^   \\       \n");
    printf("    * * *   *                                                        ^ ^^^ \\     \n");
    printf("   *  *  *  ^                                                        ^      \\      \n");
    printf("  *   *     *                                                       ^        \\    \n");
    printf("     * *    *                                                      ^ ^        \\ \n");
    printf("    *   *   *                                                     ^    ^        \n");
    printf("   *     *                                                        ^       ^       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used STAFFTHROW");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_staff_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n\n\n");
    printf("                                                           \n");
    printf("      **    +                                                        ^^         \n");
    printf("     *  * *+++*                                                     ^  ^        \n");
    printf("      **   ***                                                       ^^         \n");
    printf("    * * *   *          _______________                         ^  ^   ^  ^      \n");
    printf("   *  *  *  ^                                                          ^    ^   \n");
    printf("  *   *     *                                                          ^ ^     ^\n");
    printf("     * *    *                                                         ^   ^  ^  \n");
    printf("    *   *   *                                                         ^     ^   \n");
    printf("   *     *                                                            ^         \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 1;
    Sleep(1000);
    Fight1_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight2_Mage_Idle(int *player_hp, char Move1[], char Move2[]) {
    int action;
    if (*player_hp <= 0){
        Fight_DeathScreen(player_hp);
        return 0;
    }
    if (enemy_archer_hp <=0){
        Fight_VictoryScreen(22, player_hp);
        return 0;
    }
    if (enemy_turn == 1){
        enemy_turn = 0;
        Mage_Archer_Shot(player_hp);
    }
    else{
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    //EDITING ZONE
    printf("    **    +                                                      *     ^^    \n");
    printf("   *  * *+++*                                                    | *  ^  ^   \n");
    printf("    **   ***                                                     |  *  ^^   \n");
    printf("   * *    *                                                      |^^^^^  ^^       \n");
    printf("  * *  *  ^                                                      |  *   ^  \n");
    printf(" *  *     *                                                      | *   ^   \n");
    printf("   * *    *                                                      *   ^ ^   \n");
    printf("  *   *   *                                                         ^   ^  \n");
    printf(" *     *                                                           ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    if (item_menu == 0){
    action = Move_List(Move1, Move2);
    strcpy(Move1_temp, Move1);
    strcpy(Move2_temp, Move2);
    if (action == 1){
        enemy_turn = 1;
        if (strcmp(Move1_temp, "Firestorm") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Mage_Firestorm(player_hp);}
        if (strcmp(Move1_temp, "Thunderspell") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Mage_Thunderspell(player_hp);}
        if (strcmp(Move1_temp, "Globaleclipse") == 0){
            if (ult_charge >= 100){
                Fight2_Mage_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 2){
        enemy_turn = 1;
        if (strcmp(Move2_temp, "Firestorm") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Mage_Firestorm(player_hp);}
        if (strcmp(Move2_temp, "Thunderspell") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight2_Mage_Thunderspell(player_hp);}
        if (strcmp(Move2_temp, "Globaleclipse") == 0){
            if (ult_charge >= 100){
                Fight2_Mage_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 3){
        item_menu = 1;
        Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
    }
    return action;
    }
    else if (item_menu == 1){
        tempfunc = Fight2_Mage_Idle;
        Items(player_hp, &potions, Move1_temp, Move2_temp);
    }
    }
}
int Fight2_Mage_Firestorm(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                   ++\n");
    printf("                                  +++\n");
    printf("         +   I CAST FIRESTORM!     +   \n");
    printf("       *+++*                                  +++\n");
    printf("        ***                +++               +++++              oh yea no that's-\n");
    printf("    **   *                +++++               +++                 *     ^^    \n");
    printf("   *  *  ^                 +++                                    | *  ^  ^   \n");
    printf("    ** * *                                                        |  *  ^^   \n");
    printf("   * *   *                                                        |^^^^^  ^^  \n");
    printf("  *  *   *                                                        |  *   ^ \n");
    printf(" *   *                                                            | *   ^     \n");
    printf("    * *                                                           *   ^ ^   \n");
    printf("   *   *                                                             ^   ^  \n");
    printf(" *     *                                                            ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                                                  \\   \n");
    printf("                                                                \\ +++ \n");
    printf("         +                                                     \\ +++++ \n");
    printf("       *+++*                                                      +++  \\  \\  \n");
    printf("        ***                                                           \\ +++ \n");
    printf("    **   *                                                        \\    +++++      AAAAHHHHH!!!\n");
    printf("   *  *  ^                                                      \\+++ ^  +++  ^^  \n");
    printf("    ** * *                                                     \\+++++  ^   ^   ^       * \n");
    printf("   * *   *                                                       +++       ^ ^^        / *  \n");
    printf("  *  *   *                                                          ^        ^  ^     /  * \n");
    printf(" *   *                                                                 ^    ^     ^  /   * \n");
    printf("    * *                                                                  ^      ^   /   *   \n");
    printf("   *   *                                                           ^  ^            /   *  \n");
    printf(" *     *                                                                          *  *   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_archer_hp = enemy_archer_hp -1;
    Sleep(1000);
    Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight2_Mage_Thunderspell(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                             *   \n");
    printf("                                           * * ***   \n");
    printf("         +   I CAST THUNDERSPELL!     * *   *     **  \n");
    printf("       *+++*                         *   *        ** \n");
    printf("        ***                         *  * ** ***** **  \n");
    printf("    **   *                                 /   /                  *     ^^     \n");
    printf("   *  *  ^                                /   /                   | *  ^  ^    \n");
    printf("    ** * *                                \\  /                    |  *  ^^     \n");
    printf("   * *   *                                |  |                    |^^^^^  ^^    \n");
    printf("  *  *   *                                | /                     |  *   ^     \n");
    printf(" *   *                                    |/                      | *   ^       \n");
    printf("    * *                                                           *   ^ ^     \n");
    printf("   *   *                                                             ^   ^     \n");
    printf(" *     *                                                            ^     ^   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    printf("\n------------------------------------------------------------------------------------");
    printf("                                                                                                          *\n");
    printf("                                                                   * * ***  \n");
    printf("                                                               * *   *     ** \n");
    printf("                                                                * * ** ***** *** *  \n");
    printf("         +   I CAST THUNDERSPELL!                                 /|_  /|_  /|_ \n");
    printf("       *+++*                                                     /_ / /_ / /_ /  \n");
    printf("        ***                                                       |/   |/   |/ \n");
    printf("    **   *                                           AAAAAHHHH!!!!    ^^   \n");
    printf("   *  *  ^                                                         ^ ^  ^  ^     * \n");
    printf("    ** * *                                                          ^ ^^  ^     / * \n");
    printf("   * *   *                                                            ^^       /  * \n");
    printf("   * *   *                                                            ^^      /   * \n");
    printf("  *  *   *                                                             ^     /   * \n");
    printf(" *   *                                                                 ^    /   * \n");
    printf("    * *                                                               ^ ^  *  *  \n");
    printf("   *   *                                                             ^   ^  \n");
    printf(" *     *                                                            ^     ^ \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_archer_hp = enemy_archer_hp -1;
    Sleep(1000);
    Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight2_Mage_Ult(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                               |                                        \n");
    printf("                /       |         *                              \\                   \n");
    printf("                                    *      |       |                                  \n");
    printf("                          ---  +  *  *                                                  \n");
    printf("                      ---    +    + *   **********************           \\              \n");
    printf("                          ---  +  *  *                                                  \n");
    printf("             /                      *                               \\                     \n");
    printf("                     /            *           **              \\                              \n");
    printf("                                       *    *   *   *                      /              \n");
    printf("            \\                            *   **   *                                         \n");
    printf("                   \\             |          * *        |          /                      \n");
    printf("                                             *                                             \n");
    printf("                      \\        |             *           |      /                        \n");
    printf("                                           *   *                                        \n");
    printf("                                          *     *                                      \n");
    printf("                                    \\     *     *    /                                   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                         ***                              \n");
    printf("                                                                              \n");
    printf("                                      *       *                               \n");
    printf("                               _                     _                       \n");
    printf("                              / \\        ***        / \\                       \n");
    printf("                                  *  *   * *   *  *                      \n");
    printf("                                          *                               \n");
    printf("                                          *                               \n");
    printf("                                  *       *       *                     \n");
    printf("                          *       ---------------         *                \n");
    printf("                    *                                           *       \n");
    printf("               *      ----------------------------------------       *   \n");
    printf("            *                                                           *    \n");
    printf("         *    ------------------------------------------------------       *     \n");
    printf("       *                                                                     *   \n");
    printf("      *                                                                       *  \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                      ************       \n");
    printf("                                 *****            *****     \n");
    printf("                               ***   --------------   ***   \n");
    printf("                             **                          **  \n");
    printf("                           **   -----------------------    ** \n");
    printf("                          **                                ** \n");
    printf("                          **                                **\n");
    printf("                          ** ------------------------------ **\n");
    printf("                          **                                **\n");
    printf("                          **                                **\n");
    printf("                          **                                **\n");
    printf("                           **   -----------------------    **\n");
    printf("                            **                            ** \n");
    printf("                              **                        **  \n");
    printf("                               ***   --------------   ***   \n");
    printf("                                  *****          *****     \n");
    printf("                                      ************       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                               .     *       .       *    .       \n");
    printf("                            .    *    @    .     *     @     .    \n");
    printf("                          *   .   *  ###  *   BOOM!   ###   *  .  \n");
    printf("                             *  ++++++++   @@@@@@@@   ++++*       \n");
    printf("                          *  @@@###########@@@@@@@########@@@ *   \n");
    printf("                            **##@@@######@@@#######@@@######**    \n");
    printf("                         *  ++++@@@######@@@+++++++@@@######++  * \n");
    printf("                           @@@###############@@@@@@############@  \n");
    printf("                            *   @@@######@@@   *   @@@######@@@   \n");
    printf("                         *  @@+++########+++@@     @@+++####+++@@ \n");
    printf("                            *   .  ######  .   *     ######  . *  \n");
    printf("                         .    *     @@@     *    .    @@@    *    \n");
    printf("                            .   *   .   .     *  .    .   *    .  \n");
    printf("                         .     *     .   .    *   .    .    *     \n");
    printf("                            *     .     *   .    *    .   *    .  \n");
    printf("                           .   *      .   *    .    *     .       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_archer_hp = enemy_archer_hp -99;
    ult_charge = 0;
    Sleep(1000);
    Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Mage_Archer_Shot(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n");
    printf("                                                                               \n");
    printf("                                                                                 \n");
    printf("    **    +                                                             *  ^^    \n");
    printf("   *  * *+++*                                                          * \\^  ^   \n");
    printf("    **   ***                                                       <- *   \\^^    \n");
    printf("   * *    *                                                        <-*^^^^^^  ^^  \n");
    printf("  * * *   ^                                                        <- *   /  ^    \n");
    printf(" *  *     *                                                            * /  ^     \n");
    printf("   * *    *                                                             * ^ ^     \n");
    printf("  *   *   *                                                              ^   ^    \n");
    printf(" *     *                                                                ^     ^    \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used TRIFECTA");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_archer_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n");
    printf("                                                                            \n");
    printf("                                                                               \n");
    printf("                                                                                 \n");
    printf("    **    +                                                               \n");
    printf("   *  * *+++*                                         *      ^^                \n");
    printf("    **   ***                                         * \\    ^  ^              \n");
    printf("   * *    *             <-----------                 *  \\    ^^  ^        \n");
    printf("  * *  *  ^             <-----------                  *  \\^^^  ^^         \n");
    printf(" *  *     *             <-----------                    * *    ^            \n");
    printf("   * *    *                                                 ^ ^             \n");
    printf("  *   *   *                                               ^   ^               \n");
    printf(" *     *                                                  ^   ^^            \n");
    printf("---------------------------------------------------------------------------------------\n");
    *player_hp = *player_hp - 1;
    Sleep(1000);
    Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight3_Sword_Idle(int *player_hp, char Move1[], char Move2[]) {
    int action;
    if (*player_hp <= 0){
        Fight_DeathScreen(player_hp);
        return 0;
    }
    if (enemy_boss_hp <=0){
        Fight_VictoryScreen(13, player_hp);
        return 0;
    }
    if (enemy_turn == 1){
        enemy_turn = 0;
        if (rand() %2 == 0){ //Lets the boss use one of his 2 attacks at random
        Boss_Sword_Attack1(player_hp);}
        else{
        Boss_Sword_Attack2(player_hp);}
    }
    else{
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                      \n");
    printf("                                                         -*-       ^^       -*-       \n");
    printf("                                                      -*-   -*-   ^  ^   -*-   -*-      \n");
    printf("                                                      -*-    -*-   ^^   -*-    -*-    \n");
    printf("                                                        -*-        ^ ^       -*-      \n");
    printf("    **      /                                            -*-     ^  ^   ^   -*-     \n");
    printf("   *  *    /                                              -*-   ^    ^   ^ -*-   \n");
    printf("    **    /                                                   _/_  ^  ^               \n");
    printf("   * *   /                                                   /  / ^    ^            \n");
    printf("  * * * /                                                   /  /   ^    ^            \n");
    printf(" *  *  ^ -                                                 /  /     ^     ^         \n");
    printf("   * */                                                   /  /      \\/    \\/      \n");
    printf("  *   *                                                  /  /                    \n");
    printf(" *     *                                                 \\ /             \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    if (item_menu == 0){
    action = Move_List(Move1, Move2);
    strcpy(Move1_temp, Move1);
    strcpy(Move2_temp, Move2);
    if (action == 1){
        enemy_turn = 1;
        if (strcmp(Move1_temp, "Sprint") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Sword_Sprint(player_hp);}
        if (strcmp(Move1_temp, "HookCut") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Sword_Hook(player_hp);}
        if (strcmp(Move1_temp, "Ragnarok") == 0){
            if (ult_charge >= 100){
                Fight3_Sword_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 2){
        enemy_turn = 1;
        if (strcmp(Move2_temp, "Sprint") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Sword_Sprint(player_hp);}
        if (strcmp(Move2_temp, "HookCut") == 0){
        ult_charge += 25;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Sword_Hook(player_hp);}
        if (strcmp(Move2_temp, "Ragnarok") == 0){
            if (ult_charge >= 100){
                Fight3_Sword_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 3){
        item_menu = 1;
        Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
    }
    return action;
    }
    else if (item_menu == 1){
        tempfunc = Fight3_Sword_Idle;
        Items(player_hp, &potions, Move1_temp, Move2_temp);
    }
}
}
int Fight3_Sword_Sprint(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n");
    printf("                                                                                           \n");
    printf("                                                             -*-       ^^       -*-           \n");
    printf("                                                          -*-   -*-   ^  ^   -*-   -*-        \n");
    printf("                                                          -*-    -*-   ^^   -*-    -*-         \n");
    printf("                                          **                -*-        ^ ^       -*-          \n");
    printf("                           |             *  *                -*-     ^  ^   ^   -*-       \n");
    printf("                   --------|--^ * *  **  **                   -*-   ^    ^   ^ -*-           \n");
    printf("                           |         *   **                       _/_  ^  ^                \n");
    printf("                                   ** **    **                   /  / ^    ^                \n");
    printf("                                  **  **                        /  /   ^    ^             \n");
    printf("                               **      **                      /  /     ^     ^            \n");
    printf("                             **        **                     /  /      \\/    \\/         \n");
    printf("                           *         **                      /  /                       \n");
    printf("                                    *                        \\ /                              \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n");
    printf("                                                                                     \n");
    printf("                                                                       -*-       ^^       -*-   \n");
    printf("                                                                    -*-   -*-   ^  ^   -*-   -*-   \n");
    printf("                                                                    -*-    -*-   ^^   -*-    -*-    \n");
    printf("                                                           **         -*-        ^ ^       -*-     \n");
    printf("                                                          *  *          -*-     ^  ^   ^   -*-  \n");
    printf("                                                           **     |      -*-   ^    ^   ^ -*-    \n");
    printf("                                                         * * **^--|---------  /_  ^  ^           \n");
    printf("                                                       *   *      |         /  / ^    ^          \n");
    printf("                                                      *    *               /  /   ^    ^   \n");
    printf("                                                     *     *              /  /     ^     ^  \n");
    printf("                                                          * *            /  /      \\/    \\/ \n");
    printf("                                                         *   *          /  /       \n");
    printf("                                                       *     *          \\ /           \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_boss_hp = enemy_boss_hp -1;
    Sleep(1000);
    Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight3_Sword_Hook(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("                                                                                     \n");
    printf("                                                                  -*-       ^^       -*-  \n");
    printf("                                                               -*-   -*-   ^  ^   -*-   -*-     \n");
    printf("                                                               -*-    -*-   ^^   -*-    -*-   \n");
    printf("     **                                                          -*-        ^ ^       -*-    \n");
    printf("    *  *                                                          -*-     ^  ^   ^   -*-   \n");
    printf("     **                                                            -*-   ^    ^   ^ -*-  \n");
    printf("    * *                                                            }  _/_  ^  ^      \n");
    printf("  *  * *(-----------------------------------------------------------}/  / ^    ^         \n");
    printf(" *   *                                                             }/  /   ^    ^        \n");
    printf("*    **                                                            /  /     ^     ^    \n");
    printf("    *  *                                                          /  /      \\/    \\/    \n");
    printf("   *    *                                                        /  /   \n");
    printf("  *      *                                                       \\ / \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("                                                            \n");
    printf("               -*-       ^^       -*-                \n");
    printf("            -*-   -*-   ^  ^   -*-   -*-                 \n");
    printf("            -*-    -*-   ^^   -*-    -*-                \n");
    printf("     **       -*-        ^ ^       -*-    \n");
    printf("    *  *       -*-     ^  ^   ^   -*-        \n");
    printf("     **         -*-   ^    ^   ^ -*-           \n");
    printf("    * *             _/_  ^  ^            \n");
    printf("  *  * *           /  / ^    ^         \n");
    printf(" *   *  *         /  /   ^    ^        \n");
    printf("*    **          /  /     ^     ^    \n");
    printf("    *  *        /  /      \\/    \\/\n");
    printf("   *    *      /  /         \n");
    printf("  *      *     \\ /       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n\n\n\n\n");
    printf("                                   \n");
    printf("                           -*-        ^^      -*-    \n");
    printf("                        -*-   -*-    ^  ^  -*-   -*-  \n");
    printf("                        -*-    -*-    ^^  -*-    -*-     \n");
    printf("                          -*-        ^ ^       -*-        \n");
    printf("                           -*-     ^  ^   ^   -*-     \n");
    printf("                            -*-   ^   ^    ^ -*-     \n");
    printf("         !POW!            __        ^ ^         \n");
    printf("     **    *              /       ^   ^           \n");
    printf("    *  *   *            _/_      ^    ^    \n");
    printf("     **   *            /  /     ^     ^       \n");
    printf("    * *  *            /  /     \\/    \\/      \n");
    printf("  *   *              /  /                    \n");
    printf(" *    *             /  /               \n");
    printf("*     **           /  /        \n");
    printf("     *  *         \\ /           \n");
    printf("    *    *          \n");
    printf("  *      *       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_boss_hp = enemy_boss_hp -1;
    Sleep(1000);
    Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight3_Sword_Ult(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                         \n");
    printf("    -*-       **       -*-                                  -*-       ^^       -*-        \n");
    printf(" -*-   -*-   *  *   -*-   -*-                            -*-   -*-   ^  ^   -*-   -*-        \n");
    printf(" -*-    -*-   **   -*-    -*-                            -*-    -*-   ^^   -*-    -*-           \n");
    printf("   -*-       * *        -*-                                -*-        ^ ^       -*-         \n");
    printf("    -*-   *   *  *     -*-                                  -*-     ^  ^   ^   -*-           \n");
    printf("     -*- *   *    *   -*-                                    -*-   ^    ^   ^ -*-            \n");
    printf("      _/_   *  *  _\\_                                            _/_  ^  ^                 \n");
    printf("     /  /  *    * \\  \\                                          /  / ^    ^             \n");
    printf("    /  /  *    *   \\  \\                                        /  /   ^    ^             \n");
    printf("   /  / *     *     \\  \\                                      /  /     ^     ^             \n");
    printf("  /  /               \\  \\                                    /  /      \\/    \\/      \n");
    printf(" /  /                 \\  \\                                  /  /                          \n");
    printf(" \\ /                   \\ /                                  \\ /                           \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                          \n");
    printf("                                                           -*-       ^^       -*-      \n");
    printf("                                                        -*-   -*-   ^  ^   -*-   -*-       \n");
    printf("                                                        -*-    -*-   ^^   -*-    -*-      \n");
    printf("    -*-       **      -*-                                 -*-        ^ ^       -*-    \n");
    printf(" -*-   -*-   *  *  -*-   -*-                               -*-     ^  ^   ^   -*-            \n");
    printf(" -*-    -*-   **  -*-    -*-                                -*-   ^    ^   ^ -*-           \n");
    printf("   -*-       * *       -*-                                      _/_  ^  ^                \n");
    printf("    -*-   *   *   *   -*-                                      /  / ^    ^                 \n");
    printf("     -*- *    *    * -*-                                      /  /   ^    ^          \n");
    printf("       _|_   * *   _|_                                       /  /     ^     ^         \n");
    printf("       | |  *   *  | |                                      /  /      \\/    \\/            \n");
    printf("     \\ | | *     * | | /                                   /  /                        \n");
    printf("      \\| | *     * | |/                                    \\ /                        \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                                        -*-       ^^       -*-          \n");
    printf("                                                     -*-   -*-   ^  ^   -*-   -*-     \n");
    printf("                         ...you know I can fly right -*-    -*-   ^^   -*-    -*-     \n");
    printf("                                                       -*-        ^ ^       -*-     \n");
    printf("                                                        -*-     ^  ^   ^   -*-     \n");
    printf("                                                         -*-   ^    ^   ^ -*-        \n");
    printf("                                                             _/_  ^  ^          \n");
    printf("    -*-       **      -*-                                   /  / ^    ^            \n");
    printf(" -*-   -*-   *  *  -*-   -*-                               /  /   ^    ^               \n");
    printf(" -*-    -*-   **  -*-    -*-                              /  /     ^     ^             \n");
    printf("   -*-       * *       -*-                               /  /      \\/    \\/        \n");
    printf("    -*-   *   *   *   -*-                               /  /                             \n");
    printf("     -*- *    *    * -*-                              __\\ /                          \n");
    printf("       _|_   * *   _|_                            __ /  / /                       \n");
    printf("       | |  *   *  | |                          _/    /  /                           \n");
    printf("     \\ | | *     * | | /                   __/     /    /                             \n");
    printf("      \\| | *     * | |/                   /      /     /                             \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    ult_charge = 0;
    Sleep(1000);
    Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Boss_Sword_Attack1(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                     \n");
    printf("                                                                                 \n");
    printf("                                                                             \n");
    printf("                                                                   \n");
    printf("                *WHERE DID                                                         \n");
    printf("     **       /    HE GO...*                                            \n");
    printf("    *<<*     /                                                              \n");
    printf("     **     /                                                                     \n");
    printf("   * * *   /                                                                    \n");
    printf("  *  *  * /                                                                    \n");
    printf(" *   *   ^ -                                                                \n");
    printf("    * * /                                                                \n");
    printf("   *   *                                                              \n");
    printf(" *     *                                                       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used Peek-A-Boo");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("            / \\                                           \n");
    printf("            \\  \\       ^^       -*-                          \n");
    printf("             \\  \\     ^  ^   -*-   -*-                       \n");
    printf("              \\  \\     ^^   -*-    -*-              \n");
    printf("  *???*        \\  \\    ^ ^       -*-                \n");
    printf("     **         \\__\\ ^   ^   ^   -*-                   \n");
    printf("    *  *          \\^    ^   ^ -*-                 \n");
    printf("     **                 ^  ^                   \n");
    printf("   * * *               ^    ^                 \n");
    printf("  *  *  *              ^    ^                 \n");
    printf(" *   *                 ^     ^              \n");
    printf("    * *                \\/    \\/              \n");
    printf("   *   *                                                 \n");
    printf(" *     *     --(----------                            \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                            \n");
    printf("                       ^^       -*-                          \n");
    printf("                      ^  ^   -*-   -*-                       \n");
    printf("                       ^^   -*-    -*-              \n");
    printf("     *???*             ^ ^       -*-                \n");
    printf("     **              ^  ^   ^   -*-                   \n");
    printf("    *  *   *        ^   ^    ^ -*-                 \n");
    printf("*    **   *        _/_  ^  ^                   \n");
    printf("   *  * *         /  / ^    ^                 \n");
    printf("      *          /  /   ^    ^                 \n");
    printf("       *        /  /     ^     ^              \n");
    printf("      * *      /  /      \\/    \\/              \n");
    printf("      *   *   /  /                                  \n");
    printf("      *    *  \\ / --(----------                  \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 2;
    Sleep(1000);
    Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Boss_Sword_Attack2(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                        \n");
    printf("                                                         -*-       ^^       -*-              \n");
    printf("                                                      -*-   -*-   ^  ^   -*-   -*-         \n");
    printf("                                                      -*-    -*-   ^^   -*-    -*-    \n");
    printf("                                             _______________        ^ ^       -*-        \n");
    printf("     **       /                             /_______________|---^ ^  ^   ^  -*-         \n");
    printf("    *  *     /                                            -*-        ^   ^ -*-       \n");
    printf("     **     /                                                      ^  ^                   \n");
    printf("   * * *   /                                                      ^    ^                 \n");
    printf("  *  *  * /                                                        ^    ^                 \n");
    printf(" *   *   ^ -                                                        ^     ^              \n");
    printf("    * * /                                                           \\/    \\/              \n");
    printf("   *   *                                                                                  \n");
    printf("  *    *                                                                                     \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used SplitterShot");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                        \n");
    printf("                                                         -*-       ^^       -*-        \n");
    printf("                       *                              -*-   -*-   ^  ^   -*-   -*-   \n");
    printf("                         *   __                       -*-    -*-   ^^   -*-    -*-      \n");
    printf("                           *                 _______________        ^ ^       -*-         \n");
    printf("     **       /             *  ___          /_______________|---^ ^  ^   ^  -*-          \n");
    printf("    *  *     /              *                             -*-        ^   ^ -*-      \n");
    printf("     **     /              * __                                    ^  ^               \n");
    printf("   * * *   /             *                                        ^    ^                 \n");
    printf("  *  *  * /            *                                           ^    ^                 \n");
    printf(" *   *   ^ -                                                        ^     ^              \n");
    printf("    * * /                                                           \\/    \\/              \n");
    printf("   *   *                                                                                  \n");
    printf(" *     *                                                                           \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                         \n");
    printf("                                                         -*-       ^^       -*-            \n");
    printf("           *                                          -*-   -*-   ^  ^   -*-   -*-         \n");
    printf("             *   __                                   -*-    -*-   ^^   -*-    -*-       \n");
    printf("               *                             _______________        ^ ^       -*-        \n");
    printf("     **         *  ___                      /_______________|---^ ^  ^   ^  -*-           \n");
    printf("    *  *   *    *                                         -*-        ^   ^ -*-          \n");
    printf("*    **   *    * __                                                ^  ^                   \n");
    printf("   *  * *    *                                                    ^    ^                 \n");
    printf("      *    *                                                       ^    ^             \n");
    printf("       *                                                            ^     ^           \n");
    printf("      * *                                                           \\/    \\/           \n");
    printf("      *   *                                                                           \n");
    printf("      *     *    --(----------                                                   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 2;
    Sleep(1000);
    Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight3_Mage_Idle(int *player_hp, char Move1[], char Move2[]) {
    int action;
    if (*player_hp <= 0){
        Fight_DeathScreen(player_hp);
        return 0;
    }
    if (enemy_boss_hp <=0){
        Fight_VictoryScreen(23, player_hp);
        return 0;
    }
    else if (enemy_turn == 1){
        enemy_turn = 0;
        if (rand() %2 == 0){
        Boss_Mage_Attack1(player_hp);}
        else{
        Boss_Mage_Attack2(player_hp);}
    }
    else{
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                      \n");
    printf("                                                         -*-       ^^       -*-       \n");
    printf("          +                                           -*-   -*-   ^  ^   -*-   -*-   \n");
    printf("        *+++*                                         -*-    -*-   ^^   -*-    -*-   \n");
    printf("         ***                                            -*-        ^ ^       -*-      \n");
    printf("     **   *                                              -*-     ^  ^   ^   -*-    \n");
    printf("    *  *  ^                                               -*-   ^    ^   ^ -*-    \n");
    printf("     ** * *                                                   _/_  ^  ^                   \n");
    printf("   * *    *                                                  /  / ^    ^                 \n");
    printf("  *  *    *                                                 /  /   ^    ^                 \n");
    printf(" *   *                                                     /  /     ^     ^              \n");
    printf("    * *                                                   /  /      \\/    \\/    \n");
    printf("   *   *                                                 /  /                   \n");
    printf("  *     *                                                \\ /                \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    if (item_menu == 0){
    action = Move_List(Move1, Move2);
    strcpy(Move1_temp, Move1);
    strcpy(Move2_temp, Move2);
    if (action == 1){
        enemy_turn = 1;
        if (strcmp(Move1_temp, "Firestorm") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Mage_Firestorm(player_hp);}
        if (strcmp(Move1_temp, "Thunderspell") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Mage_Thunderspell(player_hp);}
        if (strcmp(Move1_temp, "Globaleclipse") == 0){
            if (ult_charge >= 100){
                Fight3_Mage_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 2){
        enemy_turn = 1;
        if (strcmp(Move2_temp, "Firestorm") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Mage_Firestorm(player_hp);}
        if (strcmp(Move2_temp, "Thunderspell") == 0){
        ult_charge += 15;
        if (ult_charge >= 100){
            ult_charge = 100;
        }
        Fight3_Mage_Thunderspell(player_hp);}
        if (strcmp(Move2_temp, "Globaleclipse") == 0){
            if (ult_charge >= 100){
                Fight3_Mage_Ult(player_hp);}
            else {
                enemy_turn = 0;
                printf("\nNot enough ult charge!");
                Sleep(1000);
                Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
            }
        }
    }
    else if (action == 3){
        item_menu = 1;
        Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
    }
    return action;
    }
    else if (item_menu == 1){
        tempfunc = Fight3_Mage_Idle;
        Items(player_hp, &potions, Move1_temp, Move2_temp);
    }
}
}
int Fight3_Mage_Firestorm(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                   ++\n");
    printf("                                  +++                           -*-       ^^       -*- \n");
    printf("         +   I CAST FIRESTORM!     +                         -*-   -*-   ^  ^   -*-   -*-              \n");
    printf("       *+++*                                  +++            -*-    -*-   ^^   -*-    -*-    \n");
    printf("        ***                +++               +++++             -*-        ^ ^       -*-       \n");
    printf("    **   *                +++++               +++               -*-     ^  ^   ^   -*-      \n");
    printf("   *  *  ^                 +++                                   -*-   ^    ^   ^ -*-      \n");
    printf("    ** * *                                                           _/_  ^  ^            \n");
    printf("   * *   *                                                          /  / ^    ^       \n");
    printf("  *  *   *                                                         /  /   ^    ^         \n");
    printf(" *   *                                                            /  /     ^     ^   \n");
    printf("    * *                                                          /  /      \\/    \\/   \n");
    printf("   *   *                                                        /  /      \n");
    printf(" *     *                                                        \\ /    \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                                                  \\   \n");
    printf("                                                                \\ +++ \n");
    printf("                                                               \\ +++++ \n");
    printf("                                                                  +++  \\  \\  \n");
    printf("                                                                      \\ +++ \n");
    printf("                                                                  \\    +++++      AAAAHHHHH!!!\n");
    printf("         +                                                      \\+++     +++    \n");
    printf("       *+++*                                                   \\+++++       ^^       -*-    \n");
    printf("        ***                                                      +++ -*-   ^  ^   -*-   -*-     \n");
    printf("    **   *                                                      -    -*-    ^^  -*-    -*-   \n");
    printf("   *  *  ^                                                      -*-        ^ ^       -*-  \n");
    printf("    ** * *                                                       -*-     ^  ^   ^   -*-   \n");
    printf("   * *   *                                                        -*-   ^   ^    ^ -*-    \n");
    printf("  *  *   *                                                                ^ ^          \n");
    printf(" *   *                                                                  ^   ^             \n");
    printf("    * *                                        ___________             ^    ^             \n");
    printf("   *   *                                      /           |___|       \\/   \\/         \n");
    printf(" *     *                                     /____________|   |                         \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_boss_hp = enemy_boss_hp -1;
    Sleep(1000);
    Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight3_Mage_Thunderspell(int *player_hp) {
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n");
    printf("                                             *                                       \n");
    printf("                                           * * ***          -*-       ^^       -*-       \n");
    printf("         +   I CAST THUNDERSPELL!     * *   *     **     -*-   -*-   ^  ^   -*-   -*-      \n");
    printf("       *+++*                         *   *        **     -*-    -*-   ^^   -*-    -*-            \n");
    printf("        ***                         *  * ** ***** **       -*-        ^ ^       -*-      \n");
    printf("    **   *                                 /   /            -*-     ^  ^   ^   -*          \n");
    printf("   *  *  ^                                /   /              -*-   ^    ^   ^ -*-         \n");
    printf("    ** * *                                \\  /                  _/_  ^  ^     \n");
    printf("   * *   *                                |  |                  /  / ^    ^      \n");
    printf("  *  *   *                                | /                  /  /   ^    ^        \n");
    printf(" *   *                                    |/                  /  /     ^     ^           \n");
    printf("    * *                                                      /  /      \\/    \\/      \n");
    printf("   *   *                                                    /  /            \n");
    printf(" *     *                                                    \\ /        \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------");
    printf("                                                                                    *\n");
    printf("                                                                   * * ***  \n");
    printf("                                                               * *   *     ** \n");
    printf("                                                                * * ** ***** *** *  \n");
    printf("         +   I CAST THUNDERSPELL!                                 /|_  /|_  /|_ \n");
    printf("       *+++*                                                     /_ / /_ / /_ /  \n");
    printf("        ***                                                       |/   |/   |/ \n");
    printf("    **   *                                                              ^^             \n");
    printf("   *  *  ^                                                 -*- * -*-   ^  ^   -*- * -*-    \n");
    printf("    ** * *                                          __     -*-    -*-   ^^   -*-    -*-   \n");
    printf("   * *   *                                          /        -*-   ^    ^ ^    ^  -*-       \n");
    printf("   * *   *                                        _/_         -*-   ^ ^  ^   ^   -*-     \n");
    printf("  *  *   *                                       /  /          -*-        ^     -*-    \n");
    printf(" *   *                                          /  /             -*-    ^  ^ \n");
    printf("    * *                                        /  /                    ^    ^    \n");
    printf("   *   *                                      /  /                     ^    ^       \n");
    printf(" *     *                                     /  /                      ^     ^     \n");
    printf("                                             \\ /                       \\/    \\/        \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_boss_hp = enemy_boss_hp -1;
    Sleep(1000);
    Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight3_Mage_Ult(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                               |                                        \n");
    printf("                /       |         *                              \\                   \n");
    printf("                                    *      |       |                                  \n");
    printf("                          ---  +  *  *                                                  \n");
    printf("                      ---    +    + *   **********************           \\              \n");
    printf("                          ---  +  *  *                                                  \n");
    printf("             /                      *                               \\                     \n");
    printf("                     /            *           **              \\                              \n");
    printf("                                       *    *   *   *                      /              \n");
    printf("            \\                            *   **   *                                         \n");
    printf("                   \\             |          * *        |          /                      \n");
    printf("                                             *                                             \n");
    printf("                      \\        |             *           |      /                        \n");
    printf("                                           *   *                                        \n");
    printf("                                          *     *                                      \n");
    printf("                                    \\     *     *    /                                   \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                         ***                              \n");
    printf("                                                                              \n");
    printf("                                      *       *                               \n");
    printf("                               _                     _                       \n");
    printf("                              / \\        ***        / \\                       \n");
    printf("                                  *  *   * *   *  *                      \n");
    printf("                                          *                               \n");
    printf("                                          *                               \n");
    printf("                                  *       *       *                     \n");
    printf("                          *       ---------------         *                \n");
    printf("                    *                                           *       \n");
    printf("               *      ----------------------------------------       *   \n");
    printf("            *                                                           *    \n");
    printf("         *    ------------------------------------------------------       *     \n");
    printf("       *                                                                     *   \n");
    printf("      *                                                                       *  \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                                      ************       \n");
    printf("                                 *****            *****     \n");
    printf("                               ***   --------------   ***   \n");
    printf("                             **                          **  \n");
    printf("                           **   -----------------------    ** \n");
    printf("                          **                                ** \n");
    printf("                          **                                **\n");
    printf("                          ** ------------------------------ **\n");
    printf("                          **                                **\n");
    printf("                          **                                **\n");
    printf("                          **                                **\n");
    printf("                           **   -----------------------    **\n");
    printf("                            **                            ** \n");
    printf("                              **                        **  \n");
    printf("                               ***   --------------   ***   \n");
    printf("                                  *****          *****     \n");
    printf("                                      ************       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1000);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n");
    printf("                               .     *       .       *    .       \n");
    printf("                            .    *    @    .     *     @     .    \n");
    printf("                          *   .   *  ###  *   BOOM!   ###   *  .  \n");
    printf("                             *  ++++++++   @@@@@@@@   ++++*       \n");
    printf("                          *  @@@###########@@@@@@@########@@@ *   \n");
    printf("                            **##@@@######@@@#######@@@######**    \n");
    printf("                         *  ++++@@@######@@@+++++++@@@######++  * \n");
    printf("                           @@@###############@@@@@@############@  \n");
    printf("                            *   @@@######@@@   *   @@@######@@@   \n");
    printf("                         *  @@+++########+++@@     @@+++####+++@@ \n");
    printf("                            *   .  ######  .   *     ######  . *  \n");
    printf("                         .    *     @@@     *    .    @@@    *    \n");
    printf("                            .   *   .   .     *  .    .   *    .  \n");
    printf("                         .     *     .   .    *   .    .    *     \n");
    printf("                            *     .     *   .    *    .   *    .  \n");
    printf("                           .   *      .   *    .    *     .       \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    enemy_boss_hp = enemy_boss_hp -99;
    ult_charge = 0;
    Sleep(1000);
    Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Boss_Mage_Attack1(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                        \n");
    printf("                                                       \n");
    printf("          *                                          \n");
    printf("        *+++*                                      \n");
    printf("         ***   *WHERE DID                        \n");
    printf("     **   *       HE GO...*                     \n");
    printf("    *<<*  ^                                    \n");
    printf("     ** * *                                  \n");
    printf("   * *    *                                 \n");
    printf("  *  *    *                                 \n");
    printf(" *   *                                    \n");
    printf("    * *                                   \n");
    printf("   *   *                                \n");
    printf(" *     *                                \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used Peek-A-Boo");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("            / \\                                                \n");
    printf("            \\  \\       ^^       -*-                          \n");
    printf("             \\  \\     ^  ^   -*-   -*-                       \n");
    printf("              \\  \\     ^^   -*-    -*-              \n");
    printf("  *???*        \\  \\    ^ ^       -*-                \n");
    printf("     **         \\__\\ ^   ^   ^   -*-                   \n");
    printf("    *  *          \\^    ^   ^ -*-                 \n");
    printf("     **                 ^  ^                   \n");
    printf("   * * *               ^    ^                 \n");
    printf("  *  *  *              ^    ^                 \n");
    printf(" *   *                 ^     ^              \n");
    printf("    * *                \\/    \\/              \n");
    printf("   *   *               * +                      \n");
    printf(" *     *     * * * * * * + *                  \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                \n");
    printf("                      ^^       -*-                  \n");
    printf("                     ^  ^   -*-   -*-               \n");
    printf("                      ^^   -*-    -*-              \n");
    printf("     *???*            ^ ^       -*-                \n");
    printf("     **             ^   ^   ^   -*-            \n");
    printf("    *  *   *       ^     ^    ^ -*-            \n");
    printf("*    **   *       _/_    ^  ^                   \n");
    printf("   *  * *        /  /   ^     ^                 \n");
    printf("      *         /  /     ^     ^                 \n");
    printf("       *       /  /       ^      ^              \n");
    printf("      * *     /  /        \\/     \\/              \n");
    printf("      *   *  /  /           * +                   \n");
    printf("      *    * \\ /  * * * * * * + *                                                  \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 2;
    Sleep(1000);
    Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Boss_Mage_Attack2(int *player_hp){
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                         \n");
    printf("                                                         -*-       ^^       -*-            \n");
    printf("          *                                           -*-   -*-   ^  ^   -*-   -*-    \n");
    printf("        *+++*                                         -*-    -*-   ^^   -*-    -*-     \n");
    printf("         ***                                 _______________        ^ ^       -*-        \n");
    printf("     **   *                                 /_______________|---^ ^  ^   ^  -*-           \n");
    printf("    *  *  ^                                               -*-        ^   ^ -*-         \n");
    printf("     ** * *                                                        ^  ^                   \n");
    printf("   * *    *                                                       ^    ^                 \n");
    printf("  *  *    *                                                        ^    ^                 \n");
    printf(" *   *                                                              ^     ^              \n");
    printf("    * *                                                             \\/    \\/              \n");
    printf("   *   *                                                                                  \n");
    printf(" *     *                                                                                 \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    printf("Enemy's Turn: Enemy used Splittershot");
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                     \n");
    printf("                                                         -*-       ^^       -*-             \n");
    printf("          *            *                              -*-   -*-   ^  ^   -*-   -*-       \n");
    printf("        *+++*            *   __                       -*-    -*-   ^^   -*-    -*-       \n");
    printf("         ***               *                 _______________        ^ ^       -*-       \n");
    printf("     **   *                 *  ___          /_______________|---^ ^  ^   ^  -*-      \n");
    printf("    *  *  ^                 *                             -*-        ^   ^ -*-      \n");
    printf("     ** * *                * __                                    ^  ^                   \n");
    printf("   * *    *              *                                        ^    ^                 \n");
    printf("  *  *    *            *                                           ^    ^                 \n");
    printf(" *   *                                                              ^     ^              \n");
    printf("    * *                                                             \\/    \\/      \n");
    printf("   *   *                                                                             \n");
    printf(" *     *                                                                            \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    Sleep(1200);
    system("cls");
    HP_Bar(*player_hp, enemy_boss_hp);
    //EDITING ZONE
    printf("\n------------------------------------------------------------------------------------\n\n\n");
    printf("                                                                                         \n");
    printf("                                                         -*-       ^^       -*-            \n");
    printf("           *                                          -*-   -*-   ^  ^   -*-   -*-         \n");
    printf("             *   __                                   -*-    -*-   ^^   -*-    -*-     \n");
    printf("               *                             _______________        ^ ^       -*-       \n");
    printf("     **         *  ___                      /_______________|---^ ^  ^   ^  -*-      \n");
    printf("    *  *   *    *                                         -*-        ^   ^ -*-           \n");
    printf("*    **   *    * __                                                ^  ^                   \n");
    printf("   *  * *    *                                                    ^    ^                 \n");
    printf("      *    *                                                       ^    ^                 \n");
    printf("       *                                                            ^     ^              \n");
    printf("      * *                                                           \\/    \\/              \n");
    printf("      *   *                * +                                                              \n");
    printf("      *     *    * * * * * * + *                                          \n");
    printf("---------------------------------------------------------------------------------------\n");
    //EDITING ZONE
    *player_hp = *player_hp - 2;
    Sleep(1000);
    Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp);
}
int Fight_VictoryScreen(int level, int *player_hp){
    enemy_turn = 0;
    system("cls");
    printf("  ******************************************************************************************************************\n");
    printf("  *                                                                                                                *\n");
    printf("  *        *       *       *    *           ******   ***    *       *  *****   *       ******  *******  ******     *\n");
    printf("  *        *        *     *                 *       *   *   **     **  *    *  *       *          *     *          *\n");
    printf("  *        *         *   *      *           *      *     *  * *   * *  *****   *       ******     *     ******     *\n");
    printf("  *        *          * *                   *       *   *   *  * *  *  *       *       *          *     *          *\n");
    printf("  *        ******      *        ******      ******   ***    *   *   *  *       ******  ******     *     ******     *\n");
    printf("  *                                                                                                                *\n");
    printf("  ******************************************************************************************************************\n");
    int choice;
    if (level == 13 || level == 23){ // If last boss defeated, complete game
        GameFinished();
        return 0;
    }
    else{
    printf("\n");
    printf("Would you like to: \n\n");
    printf("     1. Save your progress and continue the Journey?\n");
    printf("       2. Or save and come back later?\n");
    choice = invalidcheck_int(1, 2, "\nWhat's your decision?: ");
    switch (choice){
    case 1:
        if (level == 11){
        SaveGame(*player_hp, level, 1, ult_charge, Move1_temp, Move2_temp);
        Fight2_Sword_Idle(player_hp, Move1_temp, Move2_temp); }
        else if (level == 12){
        SaveGame(*player_hp, level, 1, ult_charge, Move1_temp, Move2_temp);
        Fight3_Sword_Idle(player_hp, Move1_temp, Move2_temp); }
        else if (level == 21){
        SaveGame(*player_hp, level, 2, ult_charge, Move1_temp, Move2_temp);
        Fight2_Mage_Idle(player_hp, Move1_temp, Move2_temp); }
        else if (level == 22){
        SaveGame(*player_hp, level, 2, ult_charge, Move1_temp, Move2_temp);
        Fight3_Mage_Idle(player_hp, Move1_temp, Move2_temp); }
        break;
    case 2:
        //save it
        if (level == 11 || level == 12 || level == 13){
        SaveGame(*player_hp, level, 1, ult_charge, Move1_temp, Move2_temp);}
        else if (level == 21 || level == 22 || level == 23){
        SaveGame(*player_hp, level, 2, ult_charge, Move1_temp, Move2_temp);}
        return 0;
        break;
    }
    }
}
int Fight_DeathScreen(int *player_hp){
    int choice;
    *player_hp = 3;
    ult_charge = 0;
    potions = 3;
    system("cls");
    printf("  ****************************************************************************************\n");
    printf("  *                                                                                      *\n");
    printf("  *      *******      *      *         *  ******       ***  *       *  ******  ******    *\n");
    printf("  *      *           * *     * *     * *  *           *   *  *     *   *       *     *   *\n");
    printf("  *      * *****    * * *    *  *   *  *  ******     *     *  *   *    ******  ******    *\n");
    printf("  *      * *   *   *     *   *   * *   *  *           *   *    * *     *       *   *     *\n");
    printf("  *      *******  *       *  *    *    *  ******       ***      *      ******  *    *    *\n");
    printf("  *                                                                                      *\n");
    printf("  ****************************************************************************************\n");
    printf("\nWould you like to: \n\n");
    printf("     1. Return to Main Menu?\n");
    printf("       2. Or Quit game?\n");
    choice = invalidcheck_int(1, 2, "\nWhat's your decision?: ");
    switch (choice){
    case 1:
        break;
    case 2:
        exit(0);
        break;
    }
}
int GameFinished(){
    int choice;
    system("cls");
    printf("  ***************************************************************************\n");
    printf("  *                                                                         *\n");
    printf("  *    *     *   ***    *    *       *       *       *  *******  *     *    *\n");
    printf("  *     *   *   *   *   *    *        *     * *     *      *     * *   *    *\n");
    printf("  *       *    *     *  *    *         *   *   *   *       *     *  *  *    *\n");
    printf("  *       *     *   *   *    *          * *     * *        *     *   * *    *\n");
    printf("  *       *      ***    ******           *       *      *******  *     *    *\n");
    printf("  *                                                                         *\n");
    printf("  ***************************************************************************\n");
    printf("\nWould you like to: \n\n");
    printf("     1. Return to Main Menu?\n");
    printf("       2. Or Quit game?\n");
    choice = invalidcheck_int(1, 2, "\nWhat's your decision?: ");
    switch (choice){
    case 1:
        break;
    case 2:
        exit(0);
        break;
    }
}
