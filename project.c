#include <stdio.h>
#include <string.h>
#include <ctype.h>

int size = 0;

// Controllers
int login(char data[][8][200]);
int signup(char data[][8][200]);
void logout(char data[][8][200]);
int take_order();
void menu(int *a, int total_price);
void complete_order(int price, char data[][8][200], int user);
void delete_user(char data[][8][200], int user);
void change_info(char data[][8][200], int user);

// Utilities
void sort(char data[][8][200]);
void swap(char a[], int ind1, int ind2);
int toNum(char str[]);

int main()
{
    FILE *database = fopen("database.txt", "r");
    char *line = NULL, data[256][8][200], row[200], *temp;
    char *token;
    size_t linesize = 0;
    int ind = 0, field = 0, user = -1, choice;
    while (getline(&line, &linesize, database) != -1)
    {
        token = strtok(line, " ");
        while (token != NULL)
        {
            strcpy(data[ind][field], token);
            if (field == 7)
            {
                data[ind][7][strlen(data[ind][7]) - 2] = '\0';
            }
            field++;
            token = strtok(NULL, " ");
        }
        field = 0;
        ind++;
    }
    size = ind - 1;
    fclose(database);
    printf("Welcome to Amazon!\n1. Login\n2. Signup\n3. Exit\n");
    scanf("%d", &choice);
    while(1){
        switch(choice){
            case 1:
                user = login(data);
                break;
            case 2:
                user = signup(data);
                break;
            case 3:
                return 0;
            default:
                printf("Please Enter a correct choice.\n");
                scanf("%d", &choice);
        }
        if(user != -1){
            break;
        }
    }
    printf("\nWelcome %s!\n", data[user][1]);
    while (1)
    {
        printf("\nWhat would you like to do?\n\n1.Change informations\n\n2.Make order\n\n3.Logout\n\n4.Delete Account and Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            change_info(data, user);
            break;
        case 2:
            complete_order(take_order(), data, user);
            break;
        case 3:
            logout(data);
            return 0;
        case 4:
            delete_user(data, user);
            logout(data);
            return 0;
        default:
            printf("Please Enter a correct choice.\n");
        }
    }
    return 0;
}

// Utilities Definitions
void swap(char a[], int ind1, int ind2)
{
    char temp;
    temp = a[ind1];
    a[ind1] = a[ind2];
    a[ind2] = temp;
}
void swapline(char data[][8][200], int a, int b)
{
    char temp[8][200];
    for (int i = 0; i < 8; i++)
    {
        strcpy(temp[i], data[a][i]);
        strcpy(data[a][i], data[b][i]);
        strcpy(data[b][i], temp[i]);
    }
}
void sort(char data[][8][200])
{
    int max = 0, cur = 1;
    for (int i = 0; i <= size; i++)
    {
        max = i;
        for (int j = cur; j <= size; j++)
        {
            if (strcmp(data[j][2], data[max][2]) < 0)
            {
                max = j;
            }
        }
        if (cur - 1 != max)
        {
            swapline(data, cur - 1, max);
        }
        cur++;
    }
}
int toNum(char str[])
{
    int num = 0, cur = 1;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        num += (str[i] - '0') * cur;
        cur *= 10;
    }
    return num;
}

// Controllers Definitions
int login(char data[][8][200])
{
    char email[200], password[200], secret[200];
    int index;
    while (1)
    {
        index = -1;
        printf("Please login:\nEmail: ");
        scanf("%s", email);
        for (int i = 0; i <= size; i++)
        {
            if (!strcmp(data[i][0], email))
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            printf("Email does not have an existing account. Please signup:\n");
            return signup(data);
        }
        printf("Password: ");
        scanf("%s", password);
        int cur = 0, isFlipped = 0;
        if (!strcmp(password, data[index][3]))
        {
            return index;
        }
        while (cur < strlen(password) - 1)
        {
            swap(password, cur, cur + 1);
            if (!strcmp(password, data[index][3]))
            {
                isFlipped += 1;
            }
            swap(password, cur, cur + 1);
            cur++;
        }
        if (isFlipped == 1)
        {
            printf("It seems you have mistyped your password. Answer your secret question:\nWhat's the name of your %s\n", data[index][6]);
            scanf("%s", secret);
            if (!strcmp(secret, data[index][7]))
            {
                return index;
            }
        }
        printf("Wrong secret answer. ");
    }
}
int signup(char data[][8][200])
{
    int choice, balance, exists = 0;
    printf("Email: ");
    while (1)
    {
        scanf("%s", data[size + 1][0]);
        for (int i = 0; i <= size; i++)
        {
            if (!strcmp(data[i][0], data[size + 1][0]))
            {
                exists = 1;
                break;
            }
        }
        if (!exists)
        {
            break;
        }
        printf("Email already exists, please enter another one: ");
        exists = 0;
    }
    printf("Password: ");
    scanf("%s", data[size + 1][3]);
    printf("First name: ");
    scanf("%s", data[size + 1][1]);
    printf("Last name: ");
    scanf("%s", data[size + 1][2]);
    printf("Bank account number: ");
    exists = 0;
    while (1)
    {
        scanf("%s", data[size + 1][4]);
        for (int i = 0; i <= size; i++)
        {
            if (!strcmp(data[i][4], data[size + 1][4]))
            {
                exists = 1;
                break;
            }
        }
        if (!exists)
        {
            break;
        }
        printf("Bank account already linked with another account, please enter another one: ");
        exists = 0;
    }
    printf("Enter your account balance: ");
    scanf("%s", data[size + 1][5]);
    printf("Choose a secret question:\n1. What's the name of your pet?\n2. What's the name of your mom?\n3. What's the name of your School?\n");
    exists = 1;
    while (exists)
    {
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            strcpy(data[size + 1][6], "pet?");
            exists = 0;
            break;
        case 2:
            strcpy(data[size + 1][6], "mom?");
            exists = 0;
            break;
        case 3:
            strcpy(data[size + 1][6], "school?");
            exists = 0;
            break;
        default:
            printf("Please enter a correct choice:\n");
        }
    }
    printf("Enter the answer to your secret question: ");
    scanf("%s", data[size + 1][7]);
    size++;
    return size;
}
void logout(char data[][8][200])
{
    sort(data);
    FILE *out = fopen("output.txt", "w");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            fprintf(out, "%s ", data[i][j]);
        }
        fprintf(out, "%s\n", data[i][7]);
    }
    for (int j = 0; j < 7; j++)
    {
        fprintf(out, "%s ", data[size][j]);
    }
    fprintf(out, "%s\n", data[size][7]);
}
void menu(int *a, int total_price)
{
    int choice;
    printf("\nThe list below presents the products and their prices: \n\n1-Books: 200MAD\n\n2-Skincare: 700MAD\n\n3-Watch: 1000MAD\n\n4-Desk: 4000MAD\n\n5-Laptop: 6000MAD.\n\nEnter your choice or -1 to exit\n\nCurrent basket cost: %dMAD\n", total_price);
    scanf("%d", a);
}
int take_order()
{
    int amount, choice, price, total_price = 0;
    menu(&choice, total_price);
    while (choice != -1)
    {
        if (choice > 5 || choice == 0 || choice < -1)
        {
            printf("Please choose a correct option:\n");
            menu(&choice, total_price);
            continue;
        }
        printf("Enter the amount: ");
        scanf("%d", &amount);
        if (choice == 1)
        {
            price = amount * 200;
        }
        else if (choice == 2)
        {
            price = amount * 700;
        }
        else if (choice == 3)
        {
            price = amount * 1000;
        }
        else if (choice == 4)
        {
            price = amount * 4000;
        }
        else if (choice == 5)
        {
            price = amount * 6000;
        }
        total_price += price;
        menu(&choice, total_price);
    }
    return total_price;
}
void complete_order(int price, char data[][8][200], int user)
{
    int balance = toNum(data[user][5]), exists, flag, bal;
    char bank[200];
    if (price <= balance)
    {
        balance -= price;
        printf("Transaction completed! Remaining balance: %dMAD.\n", balance);
        char nu[20] = "";
        while (balance > 0)
        {
            char temp[2] = {balance % 10 + '0', '\0'};
            strcat(nu, temp);
            balance /= 10;
        }
        int len = strlen(nu) - 1;
        for (int i = 0; i <= (len + 1) / 2; i++)
        {
            char temp;
            temp = nu[i];
            nu[i] = nu[len - i];
            nu[len - i] = temp;
        }
        strcpy(data[user][5], nu);
        return;
    }
    else
    {
        exists = 0;
        for (int i = 0; i < 3; i++)
        {
            printf("Insufficient funds. Please enter a new bank account:\n");
            flag = 0;
            while (i < 3)
            {
                scanf("%s", bank);
                if (!strcmp(bank, data[user][4]))
                {
                    exists = 1;
                }
                else
                {
                    for (int i = 0; i <= size; i++)
                    {
                        if (!strcmp(bank, data[i][4]))
                        {
                            exists = 1;
                            break;
                        }
                    }
                }
                if (!exists)
                {
                    break;
                }
                i++;
                if (i != 3)
                {
                    printf("Bank account already linked with another account, please enter another one: ");
                }
                exists = 0;
            }
            if (i == 3)
            {
                printf("Order dropped!\n");
                return;
            }
            printf("Enter your bank account balance:\n");
            scanf("%d", &bal);
            if (bal >= price)
            {
                bal -= price;
                printf("Transaction completed! Remaining balance: %dMAAD.\n", bal);
                char nu[20] = "";
                while (bal > 0)
                {
                    char temp[2] = {bal % 10 + '0', '\0'};
                    strcat(nu, temp);
                    bal /= 10;
                }
                int len = strlen(nu) - 1;
                for (int i = 0; i <= (len + 1) / 2; i++)
                {
                    char temp;
                    temp = nu[i];
                    nu[i] = nu[len - i];
                    nu[len - i] = temp;
                }
                strcpy(data[user][5], nu);
                strcpy(data[user][4], bank);
                return;
            }
        }
        printf("Order dropped!\n");
        return;
    }
}
void delete_user(char data[][8][200], int user)
{
    for (int i = 0; i < 8; i++)
    {
        strcpy(data[user][i], "");
    }
    if (size != user)
    {
        swapline(data, size, user);
    }
    size--;
}
void change_info(char data[][8][200], int user)
{
    char pass[200], choice[10], bank[200], email[200];
    int ch, exists, cho;
    printf("Please re-enter your password for security reasons:\n");
    while (1)
    {
        scanf("%s", pass);
        if (strcmp(pass, data[user][3]) != 0)
        {
            printf("Password incorrect. Would you like to try again?(Yes or No)");
            while (1)
            {
                scanf("%s", choice);
                for (int i = 0; choice[i]; i++)
                {
                    choice[i] = tolower(choice[i]);
                }
                if (strcmp(choice, "no") == 0)
                {
                    return;
                }
                else if (strcmp(choice, "yes") != 0)
                {
                    printf("Please enter a correct choice:\n");
                }
                else
                {
                    printf("Enter your password: ");
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("\n\nSelect the information you would like to change:\n\n1.First name\n\n2.Last name\n\n3.Password\n\n4.Bank account\n\n5.Email\n\n6.Secret question\n\nEnter -1 to cancel.\n");
        scanf("%d", &ch);
        switch (ch)
        {
        case -1:
            return;
        case 1:
            printf("Enter the new first name: (Old one: %s)\n", data[user][1]);
            scanf("%s", data[user][1]);
            break;
        case 2:
            printf("Enter the new last name: (Old one: %s)\n", data[user][2]);
            scanf("%s", data[user][1]);
            break;
        case 3:
            printf("Enter the new password:\n");
            scanf("%s", data[user][3]);
            break;
        case 4:
            printf("Enter the new bank account: (Old one: %s)\n", data[user][4]);
            exists = 0;
            while (1)
            {
                scanf("%s", bank);
                for (int i = 0; i <= size; i++)
                {
                    if (!strcmp(bank, data[i][4]))
                    {
                        exists = 1;
                        break;
                    }
                }
                if (!exists)
                {
                    break;
                }
                printf("Bank account already linked with another account, please enter another one: ");
                exists = 0;
            }
            strcpy(data[user][4], bank);
            break;
        case 5:
            printf("Enter the new email account: (Old one: %s)\n", data[user][0]);
            exists = 0;
            while (1)
            {
                scanf("%s", email);
                for (int i = 0; i <= size; i++)
                {
                    if (!strcmp(data[i][0], email))
                    {
                        exists = 1;
                        break;
                    }
                }
                if (!exists)
                {
                    break;
                }
                printf("Email already exists, please enter another one: ");
                exists = 0;
            }
            strcpy(data[user][0], email);
            break;
        case 6:
            printf("Choose a secret question:\n1. What's the name of your pet?\n2. What's the name of your mom?\n3. What's the name of your School?\n");
            exists = 1;
            while (exists)
            {
                scanf("%d", &cho);
                switch (cho)
                {
                case 1:
                    strcpy(data[user][6], "pet?");
                    exists = 0;
                    break;
                case 2:
                    strcpy(data[user][6], "mom?");
                    exists = 0;
                    break;
                case 3:
                    strcpy(data[user][6], "school?");
                    exists = 0;
                    break;
                default:
                    printf("Please enter a correct choice:\n");
                }
            }
            printf("Enter the answer to your secret question: ");
            scanf("%s", data[user][7]);
            break;
        }
    }
}