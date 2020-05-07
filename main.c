/*Bluetooth Device Management Database using a dynamic - single linked list*/

/* Define libraries to be included */
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

/* Define Functions*/
void clearInput(void);

void addNewDevice(void);

void listAllDevices(void);

void deleteDevice(void);

void modifyDevice(void);

int findDevice(void);

int prompt(void);

int checkDevice(int);

/* Define Structures*/
typedef struct bluetoothDevice {
    int serialNumber;        /*unique account serial Number*/
    char deviceName[20];     /*contains device Name*/
    struct bluetoothDevice *next; /*next is used to navigate through structures.*/
} BluetoothDevice;

BluetoothDevice *firstRecord, *currentRecord, *newRecord; /*pointers*/
/* firstRecord is used to point to first record in list, currentRecord points to current record in list, newRecord contains address of new structure/node */

int currentRecordNumber = 0; /*gives unique serial numbers*/

/* Main Function */
int main() {

    FILE *datafile;
    char *filename = "bluetoothDeviceDatabase.dat";/*declare file devices*/
    char ch;
    firstRecord = NULL;

    datafile = fopen(filename, "r");/* open file for reading*/

    if (datafile) {

        firstRecord = (struct bluetoothDevice *) malloc(sizeof(struct bluetoothDevice));
        /*use of malloc to set aside memory relative to size of structure bluetoothDevice*/
        currentRecord = firstRecord;       /*make first record current*/
        while (1) /*endless while loop. a NULL pointer in final node ends loop*/
        {
            newRecord = (struct bluetoothDevice *) malloc(sizeof(struct bluetoothDevice));
            fread(currentRecord, sizeof(struct bluetoothDevice), 1, datafile);
            if (currentRecord->next == NULL)   /* NULL indicates end of node list*/
                break;
            currentRecord->next = newRecord;       /* pointer referencing next node*/
            currentRecord = newRecord;             /* make current record new*/
        }
        fclose(datafile);                /* close file - good practice to cloe files after use*/
        currentRecordNumber = currentRecord->serialNumber;
    }

    do {
        fflush(stdin);
        puts("\nWelcome To The Bluetooth Device Database");/* print menu messages*/
        puts("-- -----------------------------");
        puts("1 - Add a new bluetooth Device");
        puts("2 - Delete bluetooth Device");
        puts("3 - List all bluetooth Devices");
        puts("4 - Modify bluetooth Device");
        puts("5 - Find a bluetooth Device by deviceName");
        puts("-- -----------------------------");
        puts("Q - Save and quit\n");
        printf("\tYour choice:");
        ch = getchar();
        ch = toupper(ch);/*changes user input case to upper case*/
        switch (ch)     /*stores in ch variable.*/
        {
            case '1':
                puts("Add a new bluetooth Device\n");
                fflush(stdin);
                addNewDevice();//call addNewDevice function
                break;
            case '2':
                puts("Delete a bluetooth Device\n");
                deleteDevice();
                break;
            case '3':
                puts("List all bluetooth Devices\n");
                listAllDevices();
                break;
            case '4':
                puts("Modify a bluetooth Device\n");
                modifyDevice();
                break;
            case '5':
                puts("Find a bluetooth Device by deviceName\n");
                findDevice();
                break;
            case 'Q':
                puts("Save and quit\n");
            default:
                break;
        }
    } while (ch != 'Q');

    /*
     * Save the records to disk
     */
    currentRecord = firstRecord;
    if (currentRecord == NULL)
        return (0);        /*no data to write*/

    datafile = fopen(filename, "w");   /*open file to write*/
    if (datafile == NULL) {
        printf("Error writing to %s\n", filename);
        return (1);
    }

    /* Write each record to disk*/
    while (currentRecord != NULL) {
        fwrite(currentRecord, sizeof(struct bluetoothDevice), 1, datafile);
        currentRecord = currentRecord->next;
    }
    fclose(datafile);             /*closes data file*/
    return (0);
}

void addNewDevice(void) /* add new bluetooth Device function*/
{
    newRecord = (struct bluetoothDevice *) malloc(sizeof(struct bluetoothDevice));
    /*allocates memory for new structure.*/

    /*
     * Checks to see whether this is the first record in file
     * If so, then all pointers are initialized to this record,
     */
    if (firstRecord == NULL)
        firstRecord = currentRecord = newRecord;
        /*
         * if not, end of structure list is obtained
         */
    else {
        currentRecord = firstRecord;      /* make the first record the current one*/

        while (currentRecord->next != NULL)currentRecord = currentRecord->next;
        /* and loop through all records*/
        currentRecord->next = newRecord;  /* pointer to next node */
        currentRecord = newRecord;        /* make current record the new one*/
    }

    /* update the structure */
    currentRecordNumber++;
    printf("%27s: %5i\n", "bluetooth Device serial Number", currentRecordNumber);
    currentRecord->serialNumber = currentRecordNumber;    /*currentRecordNumber is used to give unique account numbers*/

    printf("%27s: ", "Enter bluetooth Device deviceName");
    gets(currentRecord->deviceName);
    printf("bluetooth Device added!");

    /*
     * gives the new record a NULL pointer
     * to show it's the last record:
     */
    currentRecord->next = NULL;
}

void listAllDevices(void) /* list all contacts function*/
{
    if (firstRecord == NULL)
        puts("There are no devices to display!"); /*prints message*/

    else {

        printf("%6s %-20s\n", "#", "Device Name");
        puts("------ -------------------- ------------- -------------------");
        /*prints table titles*/
        currentRecord = firstRecord;

        do {

            printf("%6d: %-20s\n", \
                currentRecord->serialNumber, \
                currentRecord->deviceName);
            /*prints values of serial Number, device Name*/
        } while ((currentRecord = currentRecord->next) != NULL);
    }
}

void deleteDevice(void)     /*delete bluetooth Device function */
{
    int record;
    struct bluetoothDevice *previousDevice;

    if (firstRecord == NULL) {
        puts("There are no devices to delete!");
        return;
    }

    listAllDevices();        /* show all records*/
    printf("Enter bluetooth Device serial Number to delete: ");
    scanf("%d", &record);
    currentRecord = firstRecord;

    while (currentRecord != NULL) {

        if (currentRecord->serialNumber == record) {

            if (currentRecord == firstRecord)    /*if record to be deleted is the first record*/
                firstRecord = currentRecord->next; /*reset firstRecord to point at next record as first*/
            else
                previousDevice->next = currentRecord->next;/*previous pointer used if record*/

            /*to delete is not the first*/
            free(currentRecord); /*frees memory <deletes>*/
            printf("bluetooth Device %d deleted!\n", record);
            return;

        } else {

            previousDevice = currentRecord;
            currentRecord = currentRecord->next;
        }
    }
    printf("bluetooth Device %d not found!\n", record);
}

void modifyDevice(void)   /*modify bluetooth Device function*/
{
    int record, result;

    if (firstRecord == NULL) {
        puts("There are no devices to modify!");
        return;
    }

    listAllDevices();        /* show all records */
    printf("Enter bluetooth Device serial Number to modify or change: ");
    scanf("%d", &record);  /*scan user input to record*/
    result = checkDevice(record);

    if (result > 0) {

        printf("Bluetooth Device %d:\n", currentRecord->serialNumber);
        printf("Name: %s\n", currentRecord->deviceName);

        if (prompt())
            gets(currentRecord->deviceName);
        return;
    }
    printf("bluetooth Device %d was not found!\n", record);
}

int checkDevice(int recordNumber) {

    int record;
    record = recordNumber;
    currentRecord = firstRecord;

    while (currentRecord != NULL) {

        if (currentRecord->serialNumber == record) {
            return 1;
        } else {
            currentRecord = currentRecord->next;
        }
    }
    return -1;
}

int findDevice(void) /* find bluetooth Device function*/
{
    char buff[20];

    if (firstRecord == NULL) {
        puts("There are no devices to find!");
        return 1;
    }

    printf("Enter bluetooth Device deviceName: ");
    fflush(stdin);/*clears any text from the input stream*/
    gets(buff);

    currentRecord = firstRecord;

    while (currentRecord != NULL) {

        if (strcmp(currentRecord->deviceName, buff) == 0) {

            printf("%6s %-20s\n", "#", "Device Name");
            /*prints table titles*/
            printf("%6d: %-20s\n", \
            currentRecord->serialNumber, \
            currentRecord->deviceName);
            /*prints values of serial Number, device Name*/
            return 0;

        } else {
            currentRecord = currentRecord->next;
        }
    }

    printf("bluetooth Device %s was not found!\n", buff);
    return 1;
}

int prompt(void) {

    char ch;

    fflush(stdin);
    printf("Update? (Y to update any other key to not)");
    ch = getchar();
    ch = toupper(ch);
    fflush(stdin);

    if (ch == 'Y') {
        printf("Enter new value: ");
        return (1);
    } else
        return (0);
}

/* END OF PROGRAM */
