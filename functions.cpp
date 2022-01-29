//
//  functions.cpp
//  cozjesc_projekt_zaliczeniowy
//
//  Created by Aleksandra on 29/12/2021.
//

//Biblioteki potrzebne do dzialania programu

#include "functions.hpp"
using namespace std;

//zmienna determinujaca dalsze dzialanie programu, przy zmianie na 0 program sie zatrzymuje.
bool endProgram=1;

//funkcja otwierajaca plik o podanej w argumencie nazwie.
fstream openFile(string file){
    string directory=std::filesystem::current_path();
    std::fstream fileRecipes;
    fileRecipes.open(directory+"/"+file);
//sprawdzenie poprawnosci otwarcia pliku i ewentualnie tworzenie pliku z przepisami.
    if(!fileRecipes.good()){
        cout<<"Plik nie istnieje. Czy chcesz utworzyć plik z przepisami? t/n"<<endl;
        char choice;
        cin>>choice;
        if(choice=='t'){
            ofstream fileRecipes(directory+"/"+file);
            cout<<"Plik został utworzony. ";
        }
        else{
            cout<<"Program zostanie zakończony, ponieważ nie może działać bez pliku z przepisami."<<endl;
            endProgram=false;
        }
    }
    return fileRecipes;
}

//funkcja rozdzielajaca podany string podzielony danym znakiem.
vector<string> separateBy(string str, char by, bool deleteSpaces){
    vector<string> separated;
    string temp="";
    for(int i=0; i<str.length(); i++){
        if(i==str.length()-1){
            temp+=str[i];
            separated.push_back(temp);
        }
        else if(str[i]!=by){
           if(deleteSpaces==1 && str[i]==' ' && by!=' ') continue;
            temp+=str[i];
        }
        else{
            separated.push_back(temp);
            temp="";
        }
    }
    return separated;
}

//funkcja analizujaca plik z przepisami i porzadkujaca go do wektora przyjmujacego klasy recipes, ktore zawieraja nazwe, skladniki i przepis.
vector<recipes> analyseRecipeFile(fstream &file){
//sprawdzenie czy plik nie jest pusty
    file.seekg(0, ios::end);
    if(file.tellg()==0) return {};
    else{
        file.seekg(0, ios::beg);
    vector<recipes> recipe;
    string line="";
    string temp="";
    while(file){
        getline(file, line);
        if(!(line=="")){
//recipes object;
        recipes object;
//nazwa potrawy
        for(int i=6; line[i]!=39; i++){
            temp+=line[i];
        }
        object.name=temp;
        temp="";
//tablica skladnikow
        for(int i=(int)line.find("[")+1; line[i]!=']'; i++){
            temp+=line[i];
        }
        object.ingredients=separateBy(temp, ',');
        temp="";
//przepis
        for(int i=(int)line.find("recipe='")+8; line[i]!=39; i++){
            temp+=line[i];
        }
        object.recipe=temp;
        temp="";
        recipe.push_back(object);
    }
    }
    file.close();
    return recipe;
    }
}

//funkcja pozwalajaca uzytkownikowi dodawac przepisy
void addRecipe(){
    fstream file=openFile("recipesBase.txt");
    file.seekg(0, ios::end);
    string name="", ingredients="", ingredient="", recipe="";
    cout<<"Podaj nazwe przepisu: ";
    cin.clear();
    cin.sync();
    getline(cin, name);
    cout<<"Podaj skladnik i naciśnij enter. Jak już nie chcesz podać więcej składników wpisz 0: ";
    cin.sync();
    getline(cin, ingredient);
    while(ingredient.find("0")){
        ingredients+=ingredient+",";
        cout<<"Podaj kolejny składnik: ";
        cin.sync();
        getline(cin, ingredient);
    }
    ingredients.erase(ingredients.size()-1,1);
    cout<<"Podaj przepis: ";
    cin.sync();
    getline(cin, recipe);
    
    file<<"name='"<<name<<"' ingredients=["<<ingredients<<"] recipe='"<<recipe<<"'"<<endl;
    cout<<"Dodano przepis.";
    file.close();
    
    
    showBottomMenu();
}

//funkcja pozwalajaca usuwac przepisy
void deleteRecipe(){
    fstream fileTemp=openFile("recipesBase.txt");
    string name=""; string line=""; string temp="";
    vector<recipes> analysedFile=analyseRecipeFile(fileTemp);
    fstream file=openFile("recipesBase.txt");
    for(int i=0; i<analysedFile.size(); i++){
        cout<<analysedFile[i].name<<endl;
    }
    cout<<"Podaj nazwę potrawy, którą chcesz usunąć: ";
    cin.clear();
    cin.sync();
    getline(cin, name);
    string toFind="name='"+name;
    bool flag=0;
//szukanie danego przepisu w pliku
    ofstream out("outfile.txt");
    while(getline(file,line))
       {
           if(line.find(toFind)==string::npos){
               out << line << "\n";
           }
           else flag=1;
       }
    if(flag==0) cout<<"Nie znaleziono takiej potrawy.";
    else cout<<"Pomyślno usunięto przepis.";
    file.close();
    out.close();
    remove("recipesBase.txt");
    rename("outfile.txt","recipesBase.txt");
    
    showBottomMenu();
    
}

//szukanie skladnikow, ktore wystepuja w jakimkolwiek przepisie
vector <string> searchIngredients(){
    fstream fileRecipes=openFile("recipesBase.txt");
    vector<recipes> analysedFile=analyseRecipeFile(fileRecipes);
    vector <string> ingredients;
    bool flag=0;
    for(int i=0; i<analysedFile.size(); i++){
        for(int j=0; j<analysedFile[i].ingredients.size(); j++){
            flag=0;
            for(int k=0; k<ingredients.size(); k++){
                if(analysedFile[i].ingredients[j]==ingredients[k]){
                    flag=1;
                }
            }
            if(flag==0) ingredients.push_back(analysedFile[i].ingredients[j]);
        }
    }
    
    for(int i=1; (i-1)<ingredients.size(); i++){
        cout<<i<<". "<<ingredients[i-1]<<endl;
    }
    
    fileRecipes.close();
               return ingredients;
}

//funkcja pozwalajaca uzytkownikowi na wybranie skladnikow z wygenerowanej wczesniej listy i analiza stringa podanego przez uzytkownika
vector <string> chooseIngredients(vector <string> Ingredients){
    cout<<"Wypisz numery, które wybierasz po przecinku (np. 1,2,3): ";
    string chosenS="";
    regex word_regex("[0-9]+(,[0-9]+)*.");
    smatch stringR;
    cin.clear();
    cin.sync();
    getline(cin, chosenS);
    chosenS=chosenS+",";
    if(regex_match(chosenS, stringR, word_regex)){
        vector <string> chosenIngredientsString=separateBy(chosenS, ',',1);
        vector <string> chosenIngredients;
        for(int i=0; i<chosenIngredientsString.size(); i++){
            if((stoi(chosenIngredientsString[i])-1)<0 || (stoi(chosenIngredientsString[i])-1)>=Ingredients.size()) continue;
            chosenIngredients.push_back(Ingredients[stoi(chosenIngredientsString[i])-1]);
        }
        return chosenIngredients;
    }
    else{
        throw exception();
    }
}

//szukanie w pliku przepisow, ktore spelniaja skladniki podane przez uzytkownika
vector <recipes> searchRecipes(vector <string> chosenIngredients){
    vector <recipes> possibleRecipes;
    fstream fileRecipes=openFile("recipesBase.txt");
    vector<recipes> analysedFile=analyseRecipeFile(fileRecipes);
    
    int flag=0;
    for(int i=0; i<analysedFile.size(); i++){
        for(int j=0; j<analysedFile[i].ingredients.size(); j++){
            for(int k=0; k<chosenIngredients.size(); k++){ if(chosenIngredients[k]==analysedFile[i].ingredients[j]) flag++;
            }
        }
        if(analysedFile[i].ingredients.size()==flag) possibleRecipes.push_back(analysedFile[i]);
        flag=0;
    }
    
        fileRecipes.close();
    if(possibleRecipes.size()==0) cout<<"Żaden przepis nie jest możliwy do wykonania z podanych składników. "<<endl;
    else{
    cout<<"Możliwe przepisy to: "<<endl;
    for(int i=0; i<possibleRecipes.size(); i++){
        cout<<i+1<<". "<<possibleRecipes[i].name<<endl;
    }
    
//wyswietlenie przepisu na zyczenie uzytkownika
    cout<<"Czy chcesz zobaczyć przepis którejś z tych potraw? Jak tak, to podaj numer potrawy. Jeżeli nie to wpisz 0: ";
    string n="";
        cin.clear();
        cin.sync();
        getline(cin, n);
        if(!isNumber(n)) throw exception();
        else if(n.length() > 0 && (stoi(n)>0 || (stoi(n)<=possibleRecipes.size() && stoi(n)>=1))) cout<<possibleRecipes[stoi(n)-1].recipe<<endl;
        else throw exception();
    }
    return possibleRecipes;
}

//funkcja sprawdzaca czy podany string jest intem
bool isNumber(const string& str)
{
    for (char const &c : str) { //przechodzi przez kazdy element str i przechowuje go w c jako alias
        if (isdigit(c) == 0) return false;
    }
    return true;
}

//funkcja wypisujaca wszystkie wpisane przepisy
void show(){
    fstream fileRecipes=openFile("recipesBase.txt");
    fileRecipes.seekg(0, ios::end);
    if(fileRecipes.tellg()==0) cout<<"Brak przepisów.";
    else{
        fileRecipes.seekg(0, ios::beg);
    vector<recipes> analysedFile=analyseRecipeFile(fileRecipes);
    for(int i=0; i<analysedFile.size(); i++){
        cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
        cout<<"Nazwa przepisu: "<<analysedFile[i].name<<endl;
        cout<<"Składniki: ";
        for(int j=0; j<analysedFile[i].ingredients.size(); j++){ cout<<analysedFile[i].ingredients[j];
            if(j!=(analysedFile[i].ingredients.size()-1)) cout<<", ";
            else cout<<endl;
        }
        cout<<"Przepis: "<<analysedFile[i].recipe<<endl;
    
        }
    }
    fileRecipes.close();
    
    showBottomMenu();
}

//funkcja zbierajaca wszystkie funkcje sluzace do wygenerowania mozliwych posilkow z podanych skladnikow
void searchingRecipes(){
    vector <string> foundIngredients=searchIngredients();
    vector <string> chosenIngredients=chooseIngredients(foundIngredients);
    searchRecipes(chosenIngredients);
    showBottomMenu();
}


//funkcja wyswietlajaca menu i pozwalajaca uzytkownikowi wybrac co chce zrobic
void menu(){
    while(endProgram) {
        try {
    system("clear");
    cout<<R"(
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Witaj w aplikacji "Co zjeść?" Została ona stworzona,   @
@ ułatwić Ci wybór potrawy na posiłek. Możesz do niej    @
@ również dodawać własne przepisy, usuwać bieżące. Do    @
@ obsługi będzie Ci służyć klawiatura.                   @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                        MENU:                           @
@   1. Wyświetl wszystkie przepisy.                      @
@   2. Dodaj nowy przepis.                               @
@   3. Usuń przepis.                                     @
@   4. Wygeneruj możliwe posiłki z podanych składników.  @
@   5. Wyjdź                                             @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@   Co wybierasz?: )";
    string choice="";
    cin.clear();
    cin.sync();
    getline(cin, choice);

    system("clear");
    
    if(choice=="1") show();
    else if(choice=="2") addRecipe();
    else if(choice=="3") deleteRecipe();
    else if(choice=="4") searchingRecipes();
    else if(choice=="5") {
        break;
        
    }
        } catch(exception e) {
            
        }
    }
}

//funkcja wyswietlajaca dolne menu
void showBottomMenu(){
    cout<<"\n\n\n\n";
    cout<<"Jeżeli chcesz wrócić do menu głównego wpisz 'menu'. Aby zakończyć program wpisz 0.: ";
    string choice="";
    cin.clear();
    cin.sync();
    getline(cin, choice);
    if(choice=="0") endProgram=0;
    else if(choice=="menu") return;
}
    




