#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<cstdlib>
#include<ctime>
#include<iomanip>

using namespace std;

//  TIMER 
const int QUESTION_TIME = 15; 

// LIFELINES 
bool lifeline5050_used = false;
bool lifelineHint_used = false;
bool lifelineSkip_used = false;

//  UI 
void line() {
    cout << "=======================================\n";
}



//  SAVE SCORE 
void saveScore(string name, string subject, string diff, int score) {
    ofstream fout("leaderboard.txt", ios::app);
    fout << name << " " << subject << " " << diff << " " << score << endl;
    fout.close();
}

//  LEADERBOARD 
void displayleaderboard() {
    line();
    cout << "LEADERBOARD\n";
    line();

    ifstream fin("leaderboard.txt");
    if (!fin) {
        cout << "No leaderboard found\n";
      
        return;
    }

    cout << setw(15) << "Name"
        << setw(15) << "Subject"
        << setw(15) << "Difficulty"
        << setw(10) << "Score\n";

    string n, s, d;
    int sc;
    while (fin >> n >> s >> d >> sc) {
        cout << setw(15) << n
            << setw(15) << s
            << setw(15) << d
            << setw(10) << sc << endl;
    }
    fin.close();
   
}

//  HIGH SCORES 
void saveHighScoreBasic(string name, int score, string diff) {
    string names[10], diffs[10];
    int scores[10], count = 0;

    ifstream fin("high_scores.txt");
    while (fin >> names[count] >> scores[count] >> diffs[count]) count++;
    fin.close();

    names[count] = name;
    scores[count] = score;
    diffs[count] = diff;
    count++;

    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - i - 1; j++)
            if (scores[j] < scores[j + 1]) {
                swap(scores[j], scores[j + 1]);
                swap(names[j], names[j + 1]);
                swap(diffs[j], diffs[j + 1]);
            }

    ofstream fout("high_scores.txt");
    for (int i = 0; i < count && i < 5; i++)
        fout << names[i] << " " << scores[i] << " " << diffs[i] << endl;
    fout.close();
}

void viewHighScores() {
    line();
    cout << "TOP 5 HIGH SCORES\n";
    line();

    ifstream fin("high_scores.txt");
    if (!fin) {
        cout << "No high scores yet\n";
        
        return;
    }

    cout << setw(12) << "Name"
        << setw(10) << "Score"
        << setw(15) << "Difficulty\n";

    string n, d;
    int s;
    while (fin >> n >> s >> d)
        cout << setw(12) << n << setw(10) << s << setw(15) << d << endl;

    fin.close();
   
}

//  INSTRUCTIONS 
void showinstructions() {
    line();
    cout << "INSTRUCTIONS\n";
    line();
    cout << "1. Choose subject and difficulty\n";
    cout << "2. 10 questions will be asked\n";
    cout << "3. Lifelines: 50-50, Hint, Skip (once)\n";
    cout << "4. Each question has 15 seconds\n";
    cout << "5. Score will be saved\n";
   
}

//  MAIN MENU 
int displaymainmenu() {
    int c;
    do {
        line();
        cout << "          QUIZ GAME\n";
        line();
        cout << "1. Start Quiz\n";
        cout << "2. Instructions\n";
        cout << "3. Leaderboard\n";
        cout << "4. High Scores\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> c;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); }
    } while (c < 1 || c > 5);
    return c;
}

// SUBJECT
int selectsubject() {
    int s;
    do {
        line();
        cout << "          SUBJECTS\n";
        line();
        cout << "1. Computer\n2. IQ\n3. English\n";
        cout << "Select subject: ";
        cin >> s;
    } while (s<1||s>3);

    return s ;
}

// DIFFICULTY
int difficulty() {
   int d;
    do {
        line();
        cout << "          DIFFICULTY\n";
        line();
        cout << "1. Easy\n2. Medium\n3. Hard\n";
        cout << "Select difficulty: ";
        cin >> d;
    } while (d< 1 || d>3);

    return d ;
}


//  QUESTION SYSTEM (WITH TIMER) 
void askQuestion(string filename, int& correct) {

    ifstream file(filename);
    if (!file) return;

    string q[600];
    int count = 0;

    while (getline(file, q[count])) {
        if (q[count] != "") count++;
    }
    file.close();

    int index = (rand() % (count / 6)) * 6;

    line();
    cout << q[index] << endl;
    line();
    for (int i = 1; i <= 4; i++)
        cout << q[index + i] << endl;

    string correctAns = q[index + 5], guess;

    time_t startTime = time(0);

    while (true) {
        cout << "\nAnswer (a/b/c/d) or lifeline (50/h/s): ";
        cin >> guess;

        if (difftime(time(0), startTime) > QUESTION_TIME) {
            cout << "\n TIME UP! Correct answer was: " << correctAns << endl;
           
            return;
        }
        if (guess == "50") {
            if (!lifeline5050_used) {
                lifeline5050_used = true;
                cout << "50-50 Lifeline: Two options are:\n";

                // Collect options
                string options[4];
                for (int i = 1; i <= 4; i++)
                    options[i - 1] = q[index + i];  // a, b, c, d options

                // Print correct option
                cout << correctAns << endl;

                // Pick one random wrong option
                int wrongIndex;
                do {
                    wrongIndex = rand() % 4;
                } while (options[wrongIndex] == correctAns);

                cout << options[wrongIndex] << endl;
                continue;
            }
            else {
                cout << "This lifeline has been used\n";
                continue;
            }
        }

        if (guess == "h") {
            if (!lifelineHint_used) {
                lifelineHint_used = true;
                cout << "Hint: Correct option starts with: " << correctAns[0] << endl;
                continue;
            }
            else {
                cout << "This lifeline has been used\n";
                continue;
            }
        }
        if (guess == "s") {
            if (!lifelineSkip_used) {
                lifelineSkip_used = true;
                cout << "Question skipped\n";
                return;
            }
            else {
                cout << "This lifeline has been used\n";
                continue;
            }
        }


        guess[0] = tolower(guess[0]);
        if (guess == correctAns) {
            cout << "Correct!\n";
            correct++;
        }
        else {
            cout << "Wrong! Correct answer: " << correctAns << endl;
        }
       
        return;
    }
}

//  MAIN
int main() {

    srand(time(0));
    int correct = 0;

    switch (displaymainmenu()) {

    case 1: {
        int subject = selectsubject();
        int diff = difficulty();

        lifeline5050_used = lifelineHint_used = lifelineSkip_used = false;
        correct = 0;

        string file;

        for (int i = 0; i < 10; i++) {
            if (subject == 1 && diff == 1)
                file = "computer_easy.txt";
            if (subject == 1 && diff == 2)
                file = "computer_medium.txt";
            if (subject == 1 && diff == 3)
                file = "computer_hard.txt";

            if (subject == 2 && diff == 1)
                file = "iq_easy.txt";
            if (subject == 2 && diff == 2)
                file = "iq_medium.txt";
            if (subject == 2 && diff == 3) 
                file = "iq_hard.txt";

            if (subject == 3 && diff == 1)
                file = "english_easy.txt";
            if (subject == 3 && diff == 2) 
                file = "english_medium.txt";
            if (subject == 3 && diff == 3)
                file = "english_hard.txt";

            askQuestion(file, correct);
        }

        line();
        cout << "FINAL SCORE: " << correct << "/10\n";
        line();

        string name;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        string diffName;
        if (diff == 1) {
            diffName = "Easy";
        }
        else if (diff == 2) {
            diffName = "Medium";
        }
        else {
            diffName = "Hard";
        }

        string subjectName;
        if (subject == 1) {
            subjectName = "Computer";
        }
        else if (subject == 2) {
            subjectName = "IQ";
        }
        else {
            subjectName = "English";
        }


        saveScore(name, subjectName, diffName, correct);
        saveHighScoreBasic(name, correct, diffName);

       
        break;
    }

    case 2: showinstructions();
        break;
    case 3: displayleaderboard(); 
        break;
    case 4: viewHighScores();
        break;
    case 5: cout << "Exiting...\n";
        break;
    }

    return 0;
}
