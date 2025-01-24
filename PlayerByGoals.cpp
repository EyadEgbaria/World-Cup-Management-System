#include "PlayerByGoals.h"

PlayerByGoals::PlayerByGoals(int id , int goals , int cards , int gamesPlayed , int gamesTeam , bool goalkeeperFlag , shared_ptr<PlayerByGoals> previous , shared_ptr<PlayerByGoals> next ) :
        id(id) ,
        goals(goals),
        cards(cards),
        gamesPlayed(gamesPlayed),
        gamesTeam(gamesTeam),
        goalkeeperFlag(goalkeeperFlag),
        previous(previous),
        next(next)
{
}

int PlayerByGoals::getID() const{
    return this->id;
}

int PlayerByGoals::getGoals() const{
    return this->goals;
}

void PlayerByGoals::setGoals(int new_goals){
    this->goals = new_goals;
}

bool operator< (const PlayerByGoals& player_a, const PlayerByGoals& player_b){
    if(player_a.goals == player_b.goals){
        if(player_a.cards == player_b.cards)
        {
            return player_a.id < player_b.id;
        }
        return player_a.cards > player_b.cards;
    }
    return player_a.goals < player_b.goals;
}

bool operator== (const PlayerByGoals& player_a, const PlayerByGoals& player_b){
    return player_a.id == player_b.id;
}

bool operator!= (const PlayerByGoals& player_a, const PlayerByGoals& player_b){
    return !(player_a == player_b);
}

bool operator<= (const PlayerByGoals& player_a, const PlayerByGoals& player_b){
    return !(player_b < player_a);
}

bool operator>= (const PlayerByGoals& player_a, const PlayerByGoals& player_b){
    return player_b <= player_a;
}

bool operator> (const PlayerByGoals& player_a, const PlayerByGoals& player_b){
    return player_b < player_a;
}

int PlayerByGoals::getCards() const
{
    return cards;
}
bool PlayerByGoals::getGoalkeeperFlag()
{
    return goalkeeperFlag;
}
shared_ptr<PlayerByGoals> PlayerByGoals::getPrevious()
{
    return previous;
}
shared_ptr<PlayerByGoals> PlayerByGoals::getNext()
{
    return next;
}
void PlayerByGoals::setPrevious(shared_ptr<PlayerByGoals> new_prev)
{
    previous = new_prev;
}
void PlayerByGoals::setNext(shared_ptr<PlayerByGoals> new_next)
{
    next = new_next;
}
int PlayerByGoals::getGamesPlayed()
{
    return gamesPlayed;
}
int PlayerByGoals::getGamesTeam()
{
    return gamesTeam;
}
void PlayerByGoals::set_games_played(int num){
    gamesPlayed = num ;
}
void PlayerByGoals::set_games_team(int num){
    gamesTeam = num ;
}
void PlayerByGoals::set_cards_recieved(int num){
    cards = num ;
}