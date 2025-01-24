//
// Created by ameer on 12/28/2022.
//

#ifndef UNTITLED1_PLAYERBYGOALS_H
#define UNTITLED1_PLAYERBYGOALS_H

#include <memory>

using std::shared_ptr;

class PlayerByGoals
{
    private:
        int id;
        int goals;
        int cards;
        int gamesPlayed;
        int gamesTeam;
        bool goalkeeperFlag;
        shared_ptr<PlayerByGoals> previous;
        shared_ptr<PlayerByGoals> next;

    public:
        PlayerByGoals( int id , int goals , int cards , int gamesPlayed , int gamesTeam , bool goalkeeperFlag , shared_ptr<PlayerByGoals> previous = nullptr , shared_ptr<PlayerByGoals> next = nullptr);
        PlayerByGoals(const PlayerByGoals& other) = default;
        ~PlayerByGoals() = default;

        int getID() const;
        int getGoals() const;
        int getCards() const;
        int getGamesPlayed();
        int getGamesTeam();
        bool getGoalkeeperFlag();
        shared_ptr<PlayerByGoals> getPrevious();
        shared_ptr<PlayerByGoals> getNext();
        void setPrevious(shared_ptr<PlayerByGoals> new_prev);
        void setNext(shared_ptr<PlayerByGoals> new_next);
        void set_games_played(int num);
        void set_games_team(int num);
        void set_cards_recieved(int num);
        void setGoals(int new_goals);

        friend bool operator< (const PlayerByGoals& employee_a, const PlayerByGoals& employee_b);
        friend bool operator== (const PlayerByGoals& employee_a, const PlayerByGoals& employee_b);
};

bool operator!= (const PlayerByGoals& player_a, const PlayerByGoals& player_b);
bool operator<= (const PlayerByGoals& player_a, const PlayerByGoals& player_b);
bool operator>= (const PlayerByGoals& player_a, const PlayerByGoals& player_b);
bool operator> (const PlayerByGoals& player_a, const PlayerByGoals& player_b);

#endif //UNTITLED1_PLAYERBYGOALS_H
