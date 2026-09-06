#include<iostream>
#include<fstream>
#include<conio.h>
#include<iomanip>
#include<string>
using namespace std;
void Register();
bool login();
string getfield(string record, int field);
void readData(string names[], string passwords[], int &indx);
int Ispresent(string name, string password, string names[], string passwords[], int idx);

const int Max_ITEMS = 100;

int itemids[Max_ITEMS];
string itemName[Max_ITEMS];
string itemsize[Max_ITEMS];
double itemprice[Max_ITEMS];
int itemQty[Max_ITEMS];

int itemCount = 0;
double Totalsales = 0.0;



int findItem(int id);
bool checkStock(int indx, int qty);
void reduceStock(int indx, int qty);

void loadData();
void saveData();

void Header();
void Menu();
void Itemrow(int indx);
void printBill(string name, string size, int qty, double price, double total);
void Additem();
void ViewInventory();
void Searchitem();
void Sellitem();
void updateStock(int indx);



int main()
{
  system("cls");
  

  
  int ch;
  bool loggedin = false;

  while (!loggedin){
    system("cls");
    cout<<"\033[33m*====================== Cloth Buzz System ======================*\n\n\033[0m                "<<endl;
   

    cout<< "*=====================================*"<<endl;
    cout<< "1. Register User."<<endl;
    cout<< "2. Login(If you are already registered)."<<endl;
    cout<< "*=====================================*"<<endl;
    cout<< "\n\033[34mSelect option:\033[0m ";
    cin>>ch;

    if(ch == 1){
        Register();
    }
    else if(ch == 2){
        if(login()){
            cout<< "\033[32mLogin Successful Welcome! To Cloth Buzz\033[0m "<<endl;
            loggedin = true;
            cout<< "Press Any key to continue to menu.." <<endl;
            getch();
            break;
        }
        else 
        { 
            cout<< "\033[31mInvalid Username and Password! (Access Denied).\033[0m" <<endl;
            cout<< "Press Any key to Try Again."<<endl;
            getch();

        }
    }
    else
    {
           cout<< "\033[31mInvalid Option.\033[0m"<< endl;
           getch();
    }
  } 

  loadData();

  while(true){
    Header();
    Menu();
    cout<< "\033[34mSelect an Option:\033[0m ";
    cin>>ch;
    
 

    if(ch == 1){
        Additem();
        saveData();
    
    }
    else if(ch == 2){
        ViewInventory();
    }
    else if(ch == 3){
        Searchitem();
    }
    else if(ch == 4){
       
    int id;
    cout << "Enter the ID of the article to update: ";
    cin >> id;

    int indx = findItem(id);    
    if (indx == -1) {
        cout << "Article not found." << endl;
        getch();
    } else {
        updateStock(indx);      
        cout << "\nUpdated item details:\n";
        Itemrow(indx);
        saveData();
        getch();
    }
}

    else if(ch == 5){
        Sellitem();
    }
    else if(ch == 6){

         cout<< ">Total Sales so far:=======> " << Totalsales << endl;
         
         cout<< "Press any key to continue..";
         getch();
    }
    else if(ch == 7){
        saveData();
        cout<< "\033[34mExiting Cloth Buzz... Data saved.\033[0m" << endl;
        getch();
        break;
    }
    else{
        cout<< "\033[31mInvalid choice. Press any key to try again.\033[0m";
        getch();
    }
  }
   return 0;
}
string getfield(string record, int field)
{
    int commacount = 1;
    string item = "";
    for(size_t x = 0; x < record.length(); x++)
    {
        if(record[x] == ','){
            commacount = commacount + 1;
            continue;
        }
        else if(commacount == field){
            item = item + record[x];
        }
    }
    return item;
}
void readData(string names[], string passwords[], int &indx)
{
    indx = 0;
    ifstream file("ClothBuzz_items.txt");
    if(!file.is_open())
    {
        return;
    }
    string record;
    while(getline(file, record))
    {
        names[indx] = getfield (record, 1);
        passwords[indx] = getfield(record, 2);
        indx++;

        if(indx >= 100)
        break;
    }
    file.close();
}
void Register()
{  

    string user, pass;

    cout<< ">Enter username or email: ";
    cin>>user;

    cout<< ">Enter Password(include letters,No. or digits): ";
    cin>>pass;

    ofstream file("Cloth Buzz(pass).txt" , ios:: app);
    if(!file.is_open()){
        cout<< "\033[31mError opening file to register user.\033[0m" <<endl;
        return;
    }
    file << user << "," << pass <<endl;
    file.close();

    cout<< "\033[32mUser registered successfully!\033[0m" <<endl;

    cout<< "Press Any key to continue to login.." <<endl;
    getch();
} 
bool login()
{

    system("cls");
    cout<<"#======================================================#              "<<endl;
    cout<<"#==================== Cloth Buzz Login! ===============#              "<<endl;
    cout<<"#======================================================#\n\n         "<<endl;



    string username, password, user, pass;


    cout<< ">Enter Your Username or email: ";
    cin>>username;
    cout<< ">Enter Your Password: ";
    cin>>password;

    ifstream file("Cloth Buzz(pass).txt");
    if(!file.is_open()){
        return false;
    }
    string record;
    while(getline(file, record))
    {
        user = getfield(record, 1);
        pass = getfield(record, 2);

        if(user == username && pass == password){
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
int Ispresent(string name, string password, string names[], string passwords[], int idx)
{
    int indx = -1;
    for(int i = 0; i < idx; idx++)
    {
        if(name == names[i] && password == passwords[i])
        indx = i;
    }
    return indx;
}
int findItem(int id){
    for(int i = 0; i < itemCount; i++){
        if(itemids[i] == id){
            return i;
        }
    }
    return -1;
}
bool checkStock(int indx, int qty){
    return itemQty[indx] >= qty;
}

void reduceStock(int indx, int qty){
    itemQty[indx] -= qty;
}
void loadData(){
    ifstream file("ClothBuzz_items.txt");
    if(!file.is_open()){
        itemCount = 0;
        Totalsales = 0.0;
        return;
    }
    file >> itemCount;
    file >> Totalsales;
    file.ignore();
    
    int i = 0;
    string record;
    string field;
    while(getline(file, record))
    {
        itemids[i] = stoi(getfield(record, 1));
        itemName[i]= getfield(record, 2);
        itemsize[i]= getfield(record, 3);
        itemprice[i] = stof(getfield(record, 4)); 
        itemQty[i] = stof(getfield(record, 5));
        i++;
        if(i >= 100)
        break;
    }
    file.close();
    }
void saveData(){
    ofstream file("ClothBuzz_items.txt");
    if(!file.is_open()){
        cout<< "Error Saving data!" << endl;

        return;
    }
    file << itemCount << endl;
    file << Totalsales << endl;

    for(int i = 0; i < itemCount; i++){
    file << itemids[i] << "," << itemName[i] << "," << itemsize[i] << "," << itemprice[i] << "," << itemQty[i] << endl;
    }
    file.close();
    }

 void Header() {
    system("cls");
    cout<<"                                    #========================================================#          "<<endl;
    cout<<"                                    #========>>>>>>>>>>>>>  Cloth Buzz <<<<<<<<<<<<<=========#          "<<endl;
    cout<<"                                    #========================================================#          "<<endl;
    cout<< endl;
    cout<< endl;
}


void Menu(){

    
cout<<"|============== Menu ================|"<<endl; 

    cout << "1.Add Article." << endl;
    cout << "2.View Inventory." << endl;
    cout << "3.Search Article by ID." << endl;
    cout << "4.Update Stock." << endl;
    cout << "5.Buy Article." <<endl;
    cout << "6.View Total sales." << endl;
    cout << "7.Save and Exit.." << endl;
cout<<"|====================================|"<<endl;
}

void Itemrow(int indx){
    cout << "ID: " << itemids[indx] 
         << "| Name: "  << itemName[indx] 
         << "| Size: "  << itemsize[indx] 
         << "| Price: " << itemprice[indx] 
         << "| Stock: " << itemQty[indx] << endl;
}
void printBill(string name, string size, int qty, double price, double total){

    time_t now = time(0);
    char*dt = ctime(&now);
 cout <<"\n\033[32m*-------------------Total Bill---------------------*\033[0m"<<endl;
  cout << "Date & Time: " << dt;
  cout << "Item: " << name << " (" << size << ")" << endl;
  cout << "Quantity: " << qty << endl;
  cout << "Price per Item: " << price << "Rs" << endl;
  cout << "Total Amount: " << total << "Rs" << endl;
 cout << "\033[32m*--------------------------------------------------*\033[0m" <<endl;

 cout << "\033[33mThank You For Visiting Cloth Buzz! Happy Shoping.\033[0m" << endl;

   ofstream bout("Bill.txt", ios::app);
   if(!bout.is_open()){
   cout << "\033[31mError Could not open Bill.txt to print Bill.\033[0m" << endl;
   getch();
   return;
   }
bout<<"*-------------------------Bill-------------------------*" << endl;
bout << "Date & Time: " << dt;
bout << "Item: " << name << " (" << size << ")" << endl;
bout << "Quantity: " << qty << endl;
bout << "Price per Item: " << price << "Rs" << endl;
bout << "Total Amount: " << total << "Rs" << endl;
bout <<"*------------------------------------------------------*" <<endl << endl;

bout << "Thank You For Visiting Cloth Buzz! Happy Shopping." << endl;

    bout.close();

    cout << "\n\033[1mBill has also been printed to Bills.txt\033[0m" << endl;
    getch();


}
void Additem(){
    if(itemCount >= Max_ITEMS){
        cout<< "\033[34mCannot add more items inventory full.\033[0m" << endl;
        getch();
        return;
    }
    int id;
    string name, size;
    double price;
    int qty;
     

    cout << "Enter Article's ID: ";
    cin>>id;
    

    if(findItem(id) != -1){
        cout << "Item ID already exists. Try another ID." << endl;
        cout << "Press any key to continue..";
        getch();
        return;
    }
    
    cout << "Enter Article Name: " ;
    cin.ignore();
    getline(cin, name);
    cout << "Enter Article size (S/M/L/XL): ";
    getline(cin, size);
    cout << "Enter Price of article: ";
    cin>> price;
    cout << "Enter Quantity: ";
    cin>>qty;

    itemids[itemCount] = id;
    itemName[itemCount] = name;
    itemsize[itemCount] = size;
    itemprice[itemCount] = price;
    itemQty[itemCount] = qty;

    itemCount++;

    cout << "\033[32mNew Article Added successfully.\033[0m" << endl;
    getch();

}
void ViewInventory(){
    if (itemCount == 0){
        cout << "No articles in inventory." << endl;
        getch();
        return;
    }
    ifstream fin("ClothBuzz_items.txt" , ios::in);
    if(!fin.is_open()){
        cout << "Error opening File.";
        getch();
        return;
    }
    cout << "\n\033[33m*===================--INVENTORY--======================*\033[0m" << endl;
    for(int i = 0; i < itemCount; i++){
        Itemrow(i);

    }
    cout << "\033[33m*=========================================================*\033[0m" <<endl;
    getch();
    fin.close();
}
void updateStock(int indx){
    int op, qty; 
    float nprice;
    cout <<"====================="<< endl;
    cout << "1.Add Stock."<< endl;
    cout << "2.Reduce Stock."<< endl;
    cout << "3.Update price."<< endl;
    cout <<"====================="<< endl;

    cout<< "\033[34mEnter Choice:\033[0m" << endl;
    cin>>op;

    if(op == 1){
        
        cout << "Enter Quantity to Add: ";
        cin>>qty;
        itemQty[indx] += qty;
        cout <<"\033[32mStock Added Successfully.\033[0m" << endl;
    }
    else if(op == 2){
        cout << "Enter Quantity to Reduce: ";
        cin>>qty;

        if (itemQty[indx] >= qty ){    
        itemQty[indx] -= qty;
        cout <<"\033[32mStock Updated Successfully.\033[0m" << endl;
        }
        else {
        cout <<"\033[31mNot Enough Stock.\033[0m" << endl;
        }
        
    }
    else if(op == 3){
        cout << "Enter New Price: ";
        cin>>nprice;

        if(nprice > 0){
            itemprice[indx] = nprice;
            cout << "\033[32mPrice Updated Successfully!\033[0m" << endl;

        }
    
    else {
        cout <<"\033[31mInvalid price\033[0m" <<endl;
    }
}
    else {
        cout <<"\033[31mInvalid Choice.\033[0m" << endl;
    }
}

void Searchitem(){
    int id;
    cout << "Enter the ID to search: ";
    cin>>id;

    int indx = findItem(id);
    if(indx == -1){
        cout << "Article not found." << endl;

    }
    else {
        cout << "\033[32mArticle Found.\033[0m" << endl;
        Itemrow(indx);
      
    }
    getch();
}
void Sellitem(){
    int id;
    cout << "Enter article ID to Buy: ";
    cin>> id;

    int indx = findItem(id);
    if(indx == -1){
    cout << "Article not found!" << endl;
    getch();
    return;
    }
    int qty;
    cout << "Enter the Quantity to sell:";
    cin>> qty;
    
    if(qty <= 0){
    cout << "\033[31mInvalid quantity.\033[0m" << endl;
    getch();
    return; 
    }
    if(!checkStock(indx, qty)){
        cout << "\033[31mInsufficient stock.Available:\033[0m " << itemQty[indx] << endl;
        getch();
        return;
    }
    double billAmount = qty * itemprice[indx];
    reduceStock(indx, qty);
    Totalsales += billAmount;

    printBill(itemName[indx], itemsize[indx], qty, itemprice[indx], billAmount);
    getch();
}
