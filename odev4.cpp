#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "products.dat"

typedef struct {
    int id;
    char name[20];
    float price;
    int stock;
} Product;

Product *findProduct(FILE *file, int id);
void printProduct(Product p);
void deleteProduct();
void listProducts();
void updateProduct();
void addProduct();
void showMenu();

int main() {
    showMenu();
    return 0;
}

void showMenu() {
    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("1 - Add Product\n");
        printf("2 - Update Product Stock\n");
        printf("3 - List Products\n");
        printf("4 - Delete Product\n");
        printf("5 - Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                updateProduct();
                break;
            case 3:
                listProducts();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid selection. Please try again.\n");
        }
    } while (choice != 5);
}

Product *findProduct(FILE *file, int id) {
    static Product p;
    while (fread(&p, sizeof(Product), 1, file)) {
        if (p.id == id) {
            return &p;
        }
    }
    return NULL;
}

void printProduct(Product p) {
    printf("ID: %d | Name: %s | Price: %.2f | Stock: %d\n", p.id, p.name, p.price, p.stock);
}

void deleteProduct() {
    FILE *file = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!file || !temp) {
        printf("Failed to open files!\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        showMenu();
        return;
    }

    int id;
    printf("Enter product ID to delete: ");
    scanf("%d", &id);

    Product p;
    int found = 0;
    while (fread(&p, sizeof(Product), 1, file)) {
        if (p.id == id) {
            found = 1;
            continue;
        }
        fwrite(&p, sizeof(Product), 1, temp);
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("Product deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("Product not found.\n");
    }
    
    showMenu();
}

void listProducts() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Failed to open file!\n");
        showMenu();
        return;
    }

    Product p;
    while (fread(&p, sizeof(Product), 1, file)) {
        printProduct(p);
    }

    fclose(file);
    showMenu();
}

void updateProduct() {
    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) {
        printf("Failed to open file!\n");
        showMenu();
        return;
    }

    int id;
    printf("Enter product ID to update: ");
    scanf("%d", &id);

    Product *p = findProduct(file, id);
    if (p) {
        printf("Enter new stock value: ");
        scanf("%d", &p->stock);
        fseek(file, -sizeof(Product), SEEK_CUR);

        if (fwrite(p, sizeof(Product), 1, file)) {
            printf("Product stock updated successfully!\n");
        } else {
            printf("Error while updating the product.\n");
        }
    } else {
        printf("Product not found.\n");
    }

    fclose(file);
    showMenu();
}

void addProduct() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (!file) {
        printf("Failed to open file!\n");
        showMenu();
        return;
    }

    Product p;

    printf("Enter product ID: ");
    scanf("%d", &p.id);
    printf("Enter product name: ");
    scanf("%s", p.name);
    printf("Enter product price: ");
    scanf("%f", &p.price);
    printf("Enter product stock: ");
    scanf("%d", &p.stock);

    if (fwrite(&p, sizeof(Product), 1, file)) {
        printf("Product added successfully!\n");
    } else {
        printf("Error occurred while adding the product.\n");
    }

    fclose(file);
    showMenu();
}
