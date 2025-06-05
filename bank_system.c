#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer {
    int account_no;
    char customer_first_name[25];
    char customer_last_name[25];
    float balance;
};

void new_customer();
void display_customers();
void update_customer();
void deposit_amount();
void withdraw_amount();

int main() {
    int option;

    while (1) {
        printf("\n=====================================\n");
        printf("         Simple Bank System\n");
        printf("=====================================\n");
        printf("1. Register New Customer\n");
        printf("2. View All Customer Details\n");
        printf("3. Update Customer Name\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &option);

        switch (option) {
            case 1: new_customer(); break;
            case 2: display_customers(); break;
            case 3: update_customer(); break;
            case 4: deposit_amount(); break;
            case 5: withdraw_amount(); break;
            case 6: exit(0);
            default: printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

void new_customer() {
    FILE *fb, *ftxt;
    struct customer c;
    int new_account_no, exists = 0;

    printf("Enter New Account Number: ");
    scanf("%d", &new_account_no);

    fb = fopen("customer.dat", "rb");
    if (fb != NULL) {
        while (fread(&c, sizeof(struct customer), 1, fb)) {
            if (c.account_no == new_account_no) {
                exists = 1;
                break;
            }
        }
        fclose(fb);
    }

    if (exists) {
        printf("Account number already exists.\n");
        return;
    }

    fb = fopen("customer.dat", "ab");
    ftxt = fopen("customer.txt", "a");

    if (fb == NULL || ftxt == NULL) {
        printf("File error.\n");
        return;
    }

    c.account_no = new_account_no;
    printf("Enter First Name: ");
    scanf("%s", c.customer_first_name);
    printf("Enter Last Name: ");
    scanf("%s", c.customer_last_name);
    printf("Enter Initial Balance (Rs): ");
    scanf("%f", &c.balance);

    fwrite(&c, sizeof(struct customer), 1, fb);
    fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);

    printf("Customer registered successfully.\n");

    fclose(fb);
    fclose(ftxt);
}

void display_customers() {
    FILE *fb, *ftxt;
    struct customer c;

    fb = fopen("customer.dat", "rb");
    ftxt = fopen("customer.txt", "w");

    if (fb == NULL || ftxt == NULL) {
        printf("File error.\n");
        return;
    }

    printf("\nCustomer Details:\n");
    printf("-------------------------------------\n");
    while (fread(&c, sizeof(struct customer), 1, fb)) {
        printf("Account Number: %d\n", c.account_no);
        printf("Name: %s %s\n", c.customer_first_name, c.customer_last_name);
        printf("Balance: Rs. %.2f\n", c.balance);
        printf("-------------------------------------\n");

        fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);
    }

    fclose(fb);
    fclose(ftxt);
}

void update_customer() {
    FILE *fb;
    struct customer c;
    int account_no, found = 0;

    fb = fopen("customer.dat", "rb+");
    if (fb == NULL) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number to Update: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Name: %s %s\n", c.customer_first_name, c.customer_last_name);

            printf("Enter New First Name: ");
            scanf("%s", c.customer_first_name);
            printf("Enter New Last Name: ");
            scanf("%s", c.customer_last_name);

            fseek(fb, -sizeof(struct customer), SEEK_CUR);
            fwrite(&c, sizeof(struct customer), 1, fb);
            printf("Customer name updated successfully.\n");
            break;
        }
    }

    fclose(fb);

    if (!found) {
        printf("Account not found.\n");
    }

    display_customers();
}

void deposit_amount() {
    FILE *fb;
    struct customer c;
    int account_no, found = 0;
    float amount;

    fb = fopen("customer.dat", "rb+");
    if (fb == NULL) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number to Deposit Into: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Balance: Rs. %.2f\n", c.balance);
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid deposit amount.\n");
                fclose(fb);
                return;
            }

            c.balance += amount;
            fseek(fb, -sizeof(struct customer), SEEK_CUR);
            fwrite(&c, sizeof(struct customer), 1, fb);
            printf("Deposit successful. New Balance: Rs. %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);

    if (!found) {
        printf("Account not found.\n");
    }
}

void withdraw_amount() {
    FILE *fb;
    struct customer c;
    int account_no, found = 0;
    float amount;

    fb = fopen("customer.dat", "rb+");
    if (fb == NULL) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number to Withdraw From: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Balance: Rs. %.2f\n", c.balance);
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount <= 0 || amount > c.balance) {
                printf("Invalid withdrawal amount.\n");
                fclose(fb);
                return;
            }

            c.balance -= amount;
            fseek(fb, -sizeof(struct customer), SEEK_CUR);
            fwrite(&c, sizeof(struct customer), 1, fb);
            printf("Withdrawal successful. New Balance: Rs. %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);

    if (!found) {
        printf("Account not found.\n");
    }
}
