#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer {
    int account_no;
    char customer_first_name[25];
    char customer_last_name[25];
    float balance;
};

void newcustomer();
void customerdetails();
void update_customerdetails();
void deposit();
void withdraw();

int main() {
    int option;

    while (1) {
        printf("\n===== Simple Bank Management System =====\n");
        printf("1. Register New Customer\n");
        printf("2. View All Customer Details\n");
        printf("3. Update Customer Name\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Exit\n");
        printf("Enter your choice (1-6): ");
        scanf("%d", &option);

        switch (option) {
            case 1: newcustomer(); break;
            case 2: customerdetails(); break;
            case 3: update_customerdetails(); break;
            case 4: deposit(); break;
            case 5: withdraw(); break;
            case 6: 
                printf("Thank you for using the banking system. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    }

    return 0;
}

void newcustomer() {
    FILE *fb, *ftxt;
    struct customer c;
    int new_account_no;
    int exists = 0;

    printf("\nEnter new Account Number: ");
    scanf("%d", &new_account_no);

    fb = fopen("userdata.dat", "rb");
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
        printf("Account number already exists! Try a different number.\n");
        return;
    }

    fb = fopen("userdata.dat", "ab");
    ftxt = fopen("userdata.txt", "a");

    if (fb == NULL || ftxt == NULL) {
        printf("File error!\n");
        return;
    }

    c.account_no = new_account_no;
    printf("Enter First Name: ");
    scanf("%s", c.customer_first_name);
    printf("Enter Last Name: ");
    scanf("%s", c.customer_last_name);
    printf("Enter Initial Balance: ");
    scanf("%f", &c.balance);

    fwrite(&c, sizeof(struct customer), 1, fb);
    fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);

    printf("Customer registered successfully!\n");

    fclose(fb);
    fclose(ftxt);
}

void customerdetails() {
    FILE *fb, *ftxt;
    struct customer c;

    fb = fopen("userdata.dat", "rb");
    ftxt = fopen("userdata.txt", "w");

    if (fb == NULL || ftxt == NULL) {
        printf("Unable to open files.\n");
        return;
    }

    printf("\n=== Customer List ===\n");
    while (fread(&c, sizeof(struct customer), 1, fb)) {
        printf("Account No: %d\n", c.account_no);
        printf("Name      : %s %s\n", c.customer_first_name, c.customer_last_name);
        printf("Balance   : Rs. %.2f\n", c.balance);
        printf("-----------------------------\n");

        fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);
    }

    fclose(fb);
    fclose(ftxt);
}

void update_customerdetails() {
    FILE *fb;
    struct customer c;
    int account_no, found = 0;

    fb = fopen("userdata.dat", "rb+");
    if (fb == NULL) {
        printf("File error!\n");
        return;
    }

    printf("\nEnter Account Number to update: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Name: %s %s\n", c.customer_first_name, c.customer_last_name);
            printf("Enter new First Name: ");
            scanf("%s", c.customer_first_name);
            printf("Enter new Last Name: ");
            scanf("%s", c.customer_last_name);

            fseek(fb, -sizeof(struct customer), SEEK_CUR);
            fwrite(&c, sizeof(struct customer), 1, fb);

            printf("Customer name updated.\n");
            break;
        }
    }

    fclose(fb);

    if (!found) {
        printf("Account not found.\n");
    }
}

void deposit() {
    FILE *fb;
    struct customer c;
    int account_no, found = 0;
    float amount;

    fb = fopen("userdata.dat", "rb+");
    if (fb == NULL) {
        printf("File error!\n");
        return;
    }

    printf("\nEnter Account Number to deposit into: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Balance: Rs. %.2f\n", c.balance);
            printf("Enter amount to deposit: ");
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

void withdraw() {
    FILE *fb;
    struct customer c;
    int account_no, found = 0;
    float amount;

    fb = fopen("userdata.dat", "rb+");
    if (fb == NULL) {
        printf("File error!\n");
        return;
    }

    printf("\nEnter Account Number to withdraw from: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(struct customer), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Balance: Rs. %.2f\n", c.balance);
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount <= 0 || amount > c.balance) {
                printf("Invalid or insufficient funds.\n");
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
