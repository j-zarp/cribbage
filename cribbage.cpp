#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using namespace std;

class Card {
  public:
    Card(string rank_, char suit_) {
      transform(rank_.begin(),rank_.end(),rank_.begin(),::toupper);
      if(rank_!="2" && rank_!="3" && rank_!="4" && rank_!="5"
         && rank_!="6" && rank_!="7" && rank_!="8" && rank_!="9"
         && rank_!="10" && rank_!="J" && rank_!="K" && rank_!="Q"
         && rank_!="A"){
           throw invalid_argument("Error: rank not valid");
      }
      this->rank = rank_;
      
      suit_ = (char) toupper(suit_);
      if(suit_!='H' && suit_!='D'
         && suit_!='C' && suit_!='S'){
           throw invalid_argument("Error: suit not valid");
      }
      this->suit = suit_;
      
      if(rank_=="A") {
        this->value=1;
        this->position=1;
      }
      else if(rank_=="K" or rank_=="Q" or rank_=="J") {
        this->value=10;
        if(rank_=="K"){
          this->position=13;
        }
        else if(rank_=="Q"){
          this->position=12;
        }
        else {
          this->position=11;
        }
      }
      else {
        this->value=stoi(rank_);
        this->position=stoi(rank_);
      }
    }
    ~Card() {}
    void print() {
      cout << this->rank << this->suit << " ";
    }
    
    // public attributes
    char suit;
    string rank;
    int value;
    int position;
};

void subsequences(vector<int> a, int start, int n, vector<int*>& prev, vector<vector<int*> >& res){
  if(n==0){
    res.push_back(prev);
  }
  else {
    for(int k=start; k<a.size(); k++){
      vector<int*> prev2(prev.size()+1);
      for(int i=0; i<prev.size(); i++){
        prev2[i] = new int(*prev[i]);
      }
      prev2[prev.size()] = new int(a[k]);
      subsequences(a,k+1,n-1,prev2,res);
      prev2.clear();
    }
  }
}

bool contains_all(const vector<vector<int*> >& biglst, const vector<int*>& sublst){
  if(biglst.size()==0){
    return false;
  }
  int n = sublst.size();
  int sz;
  bool res = false;
  for(int i=0; i<biglst.size(); i++){
    sz = biglst[i].size();
    if(sz<n){
      continue;
    }
    for(int offset=0; offset<=sz-n; offset++){
      bool isthesame = true;
      for(int j=0; j<n; j++){
        if(*(biglst)[i][j+offset]!=*(sublst)[j]){
          isthesame = false;
        }
      }
      res = res or isthesame;
    }
  }
  return res;
}

bool contains(Card carte, const vector<Card>& deck){
  for(int k=0; k<deck.size(); k++){
    if(carte.suit==deck[k].suit and carte.rank==deck[k].rank){
      return true;
    }
  }
  return false;
}

void assert_no_doubles(const vector<Card>& deck){
  for(int i=0; i<deck.size(); i++){
    for(int j=i+1; j<deck.size(); j++){
      if(deck[i].suit==deck[j].suit and deck[i].rank==deck[j].rank){
        throw invalid_argument("Error: found duplicates in your deck");;
      }
    }
  }
}

void assert_no_doubles(const vector<Card>& deck1, const vector<Card>& deck2){
  vector<Card> deck = deck1;
  deck.insert(deck.end(),deck2.begin(),deck2.end());
  assert_no_doubles(deck);
}

void assert_no_doubles(const vector<Card>& deck1, const vector<Card>& deck2, const vector<Card>& deck3){
  vector<Card> deck = deck1;
  deck.insert(deck.end(),deck2.begin(),deck2.end());
  deck.insert(deck.end(),deck3.begin(),deck3.end());
  assert_no_doubles(deck);
}

int count_points(vector<Card>& deck, const vector<vector<vector<int*> > >& combi, bool prep_round, bool out){
  //DEBUG START
  /*
  for(int k=0; k<deck.size(); k++){
    deck[k].print();
  }
  cout << endl;
  for(int k=0; k<combi.size(); k++){
    for(int i=0; i<combi[k].size(); i++){
      for(int j=0; j<combi[k][i].size(); j++){
        deck[*(combi)[k][i][j]].print();
      }
      cout << endl;
    }
    cout << endl;
  }
  exit(0);
  //DEBUG FIN
  */
  int total = 0;
  int n = deck.size();
  // order the deck in ascending order
  vector<Card> deckorder;
  vector<Card> tempdeck=deck;
  int min_pos;
  int min_idx;
  while(tempdeck.size()>0){
    min_pos = 999;
    min_idx = 999;
    for(int i=0; i<tempdeck.size(); i++){
      if(tempdeck[i].position<min_pos){
        min_pos = tempdeck[i].position;
        min_idx = i;
      }
    }
    deckorder.push_back(tempdeck[min_idx]);
    tempdeck[min_idx] = tempdeck[tempdeck.size()-1];
    tempdeck.pop_back();
  }
  // find all the fifteens
  for(int k=0; k<combi.size(); k++){
    for(int i=0; i<combi[k].size(); i++){
      int val = 0;
      for(int j=0; j<combi[k][i].size(); j++){
        val += deckorder[*(combi)[k][i][j]].value;
      }
      if(val==15){
        total+=2;
        if(out){
          cout << "Fifteen " << total << endl;
        }
      }
    }
  }
  // find the straights
  vector<vector<int*> > straights;
  for(int k=combi.size()-1; k>0; k--){
    for(int i=0; i<combi[k].size(); i++){
      bool straight = true;
      for(int j=1; j<combi[k][i].size(); j++){
        if(deckorder[*(combi)[k][i][j]].position!=deckorder[*(combi)[k][i][j-1]].position+1){
          straight = false;
        }
      }
      if(straight and !contains_all(straights,combi[k][i])){
        straights.push_back(combi[k][i]);
        total += combi[k][i].size();
        if(out){
          cout << "Straight of length: " << combi[k][i].size() << endl;
        }
      }
    }
  }
  // find pairs
  for(int i=0; i<combi[0].size(); i++){
    bool kind = true;
    for(int j=1; j<combi[0][i].size(); j++){
      if(deckorder[*(combi)[0][i][j]].rank!=deckorder[*(combi)[0][i][0]].rank){
        kind = false;
      }
    }
    if(kind){
      total += combi[0][i].size();
      if(out){
        cout << combi[0][i].size() << " of a kind" << endl;
      }
    }
  }
  // check for flush
  bool flush = true;
  for(int k=1; k<n-1; k++){
    if(deck[k].suit!=deck[0].suit){
      flush = false;
    }
  }
  if(flush){
    if(out){
      cout << "Flush!" << endl;
    }
    if(deck[deck.size()-1].suit==deck[0].suit){
      total += n;
    } else {
      total += n-1;
    }
  }
  // check for nobs
  for(int k=0; k<n-1; k++){
    if(deck[k].rank=="J"){
      if(deck[deck.size()-1].suit==deck[k].suit){
        total += 1;
        if(out){
          cout << "You have nobs!" << endl;
        }
      }
    }
  }
  return total;
}

vector<string> split(string str){
  vector<string> vect;
  stringstream ss(str);
  string token;
  while(getline(ss, token, ',')) {
    vect.push_back(token);
  }
  return vect;
}

int main(int argc, char **argv) {
  vector<string> ranks;
  ranks.push_back("A");
  ranks.push_back("2");
  ranks.push_back("3");
  ranks.push_back("4");
  ranks.push_back("5");
  ranks.push_back("6");
  ranks.push_back("7");
  ranks.push_back("8");
  ranks.push_back("9");
  ranks.push_back("10");
  ranks.push_back("J");
  ranks.push_back("Q");
  ranks.push_back("K");
  vector<char> suits;
  suits.push_back('H');
  suits.push_back('D');
  suits.push_back('S');
  suits.push_back('C');
  
  // pile of a 52 cards
  vector<Card> pile;
  for(int i=0; i<suits.size(); i++){
    for(int j=0; j<ranks.size(); j++){
      pile.push_back(Card(ranks[j],suits[i]));
    }
  }
  // determine who is the dealer
  bool dealer = false;
  string ans;
  char ans_char;
  do {
    cout << "Are you the dealer ([Y/N]): ";
    cin >> ans;
    ans_char = toupper(ans.at(0));
    if(ans_char=='Y'){
      dealer = true;
    }
  } while(ans_char!='Y' && ans_char!='N');
  // get personal deck
  vector<Card> deck;
  bool is_ok;
  do {
    try {
      deck.clear();
      cout << endl << "What cards do you have?" << endl;
      cout << "The format should be e.g. 10H,5S,7D,8C,6S,KD" << endl;
      cin >> ans;
      vector<string> tokens = split(ans);
      if(tokens.size()!=6){
        throw invalid_argument("Error: you shoud provide 6 cards");
      }
      for(int k=0; k<tokens.size(); k++){
        if(tokens[k].size()>2){
          deck.push_back(Card(tokens[k].substr(0,2),char(tokens[k][2])));
        } else {
          deck.push_back(Card(string(1,tokens[k][0]),char(tokens[k][1])));
        }
      }
      assert_no_doubles(deck);
      is_ok = true;
    } catch(exception& e) {
      cerr << e.what() << endl;
      is_ok = false;
    }
  } while(!is_ok);
  vector<int> deck_idx;
  for(int i=0; i<deck.size(); i++){
    deck_idx.push_back(i);
  }
  vector<vector<int*> > combinations;
  vector<int*> empty;
  subsequences(deck_idx,0,4,empty,combinations);
  // combinations only on 5 cards to count the points
  deck_idx.pop_back();
  vector<vector<vector<int*> > > all_combi;
  for(int k=2; k<=5; k++){
    vector<vector<int*> > comb; 
    subsequences(deck_idx,0,k,empty,comb);
    all_combi.push_back(comb);
    empty.clear();
    comb.clear();
  }
  // DEBUG
  /*
  for(int k=0; k<all_combi.size(); k++){
    for(int i=0; i<all_combi[k].size(); i++){
      for(int j=0; j<all_combi[k][i].size(); j++){
        deck[*(all_combi)[k][i][j]].print();
      }
      cout << endl;
    }
    cout << endl;
  }
  cout << endl;
  // DEBUG FIN
  */
  vector<double> weights(combinations.size());
  for(int k=0; k<combinations.size(); k++){
    printf("%2.0f %%\n",float(k)/combinations.size()*100.);
    for(int i=0; i<pile.size(); i++){
      if(contains(pile[i],deck)){
        continue;
      }
      vector<Card> deck_test;
      for(int j=0; j<combinations[k].size(); j++){
        deck_test.push_back(deck[*(combinations)[k][j]]);
      }
      deck_test.push_back(pile[i]);
      vector<Card> discarded;
      for(int j=0; j<deck.size(); j++){
        if(!contains(deck[j],deck_test)){
          discarded.push_back(deck[j]);
        }
      }
      weights[k] += count_points(deck_test,all_combi,true,false)*1./(52.-deck.size());
      for(int ii=0; ii<pile.size(); ii++){
        for(int jj=0; jj<pile.size(); jj++){
          if(contains(pile[ii],deck_test) or contains(pile[jj],deck_test) or ii==jj){
            continue;
          }
          vector<Card> deck_test2 = discarded;
          deck_test2.push_back(pile[ii]);
          deck_test2.push_back(pile[jj]);
          deck_test2.push_back(pile[i]);
          if(dealer){
            weights[k] += count_points(deck_test2,all_combi,true,false)*1./99452.;
          } else {
            weights[k] -= count_points(deck_test2,all_combi,true,false)*1./99452.;
          }
        }
      }
    }
  }
  cout << endl;
  // keep combinations with highest expectancy
  double max_val = -999.;
  int max_idx = 0;
  for(int k=0; k<weights.size(); k++){
    if(weights[k]>max_val){
      max_val = weights[k];
      max_idx = k;
    }
  }
  vector<Card> comb_best;
  for(int k=0; k<combinations[max_idx].size(); k++){
    comb_best.push_back(deck[*(combinations)[max_idx][k]]);
  }
  vector<Card> crib;
  cout << endl << "discard cards: ";
  for(int k=0; k<deck.size(); k++){
    if(!contains(deck[k],comb_best)){
      crib.push_back(deck[k]);
      deck[k].print();
    }
  }
  cout << endl << endl;
  // return the additional card
  if(dealer){
    cout << "Now the opponent has to flip a card..." << endl;
  } else {
    cout << "Now we have to flip a card..." << endl;
  }
  vector<Card> temp_hand;
  do {
    try {
      temp_hand.clear();
      cout << "What card was flipped? ";
      cin >> ans;
      vector<string> tokens2 = split(ans);
      if(tokens2.size()!=1){
        throw invalid_argument("Error: you shoud provide 1 card");
      }
      for(int k=0; k<tokens2.size(); k++){
        if(tokens2[k].size()>2){
          temp_hand.push_back(Card(tokens2[k].substr(0,2),char(tokens2[k][2])));
        } else {
          temp_hand.push_back(Card(string(1,tokens2[k][0]),char(tokens2[k][1])));
        }
      }
      assert_no_doubles(temp_hand,crib,comb_best);
      is_ok = true;
    } catch (exception& e) {
      cerr << e.what() << endl;
      is_ok = false;
    }
  } while(!is_ok);
  comb_best.insert(comb_best.end(),temp_hand.begin(),temp_hand.end());
  // check for nobs
  if(comb_best[comb_best.size()-1].rank == "J"){
    if(dealer){
      cout << endl << "--> You get 2 points for the nobs!" << endl;
    } else {
      cout << endl << "--> Opponent gets 2 points for the nobs!" << endl;
    }
  }
  cout << endl << "Your hand is now:" << endl;
  for(int k=0; k<comb_best.size(); k++){
    comb_best[k].print();
  }
  cout << endl << endl;
  // count the points
  double total = count_points(comb_best,all_combi,false,true);
  cout << "--> Your total is: " << total << endl << endl;
  // count the points in the crib
  if(dealer){
    cout << "You already have in your crib: ";
    crib[0].print();
    crib[1].print();
    do {
      try {
        temp_hand.clear();
        cout << endl << "What are the two new cards in your crib? ";
        cin >> ans;
        vector<string> tokens3 = split(ans);
        if(tokens3.size()!=2){
          throw invalid_argument("Error: you shoud provide 2 cards");
        }
        for(int k=0; k<tokens3.size(); k++){
          if(tokens3[k].size()>2){
            temp_hand.push_back(Card(tokens3[k].substr(0,2),char(tokens3[k][2])));
          } else {
            temp_hand.push_back(Card(string(1,tokens3[k][0]),char(tokens3[k][1])));
          }
        }
        assert_no_doubles(temp_hand,crib,comb_best);
        is_ok = true;
      } catch(exception& e) {
        cerr << e.what() << endl;
        is_ok = false;
      }
    } while(!is_ok);
    crib.insert(crib.end(),temp_hand.begin(),temp_hand.end());
    crib.push_back(comb_best[comb_best.size()-1]);
    cout << endl << "Your hand is now:" << endl;
    for(int k=0; k<crib.size(); k++){
      crib[k].print();
    }
    cout << endl << endl;
    total = count_points(crib,all_combi,false,true);
    cout << "--> Your total is: " << total << endl << endl;
  }
  // free memory
  for(int i=0; i<combinations.size(); i++){
    for(int j=0; j<combinations[i].size(); j++){
      delete combinations[i][j];
    }
  }
  combinations.clear();
  for(int k=0; k<all_combi.size(); k++){
    for(int i=0; i<all_combi[k].size(); i++){
      for(int j=0; j<all_combi[k][i].size(); j++){
        delete all_combi[k][i][j];
      }
    }
  }
  all_combi.clear();
  return 0;
}
