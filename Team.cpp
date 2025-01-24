#include "Team.h"

Team::Team(int id, int points):
        team_id(id),
        xp(points),
        points(points),
        numPlayers(0),
        numGoalkeeprs(0),
        is_valid(false),
        gamesPlayed(0),
        id_highest_scorer(0),
        cards_highest_scorer(0),
        goals_highest_scorer(0),
        team_players_by_id_tree(new Tree<PlayerByID>),
        team_players_by_goals_tree(new Tree<PlayerByGoals>),
        valid_team(nullptr)
{
}

int Team::getID()
{
    return this->team_id;
}

int Team::getxp()
{
    return this->xp;
}

shared_ptr<validTeams> Team::getValidTeam()
{
    return this->valid_team;
}

void Team::setxp(int new_xp)
{
    xp = new_xp;
}

void Team::addxp(int to_add)
{
    xp = xp + to_add;
}

void Team::setValidTeam(shared_ptr<validTeams> new_valid)
{
    valid_team = new_valid;
}

bool operator< (const Team& team_a, const Team& team_b)
{
    return team_a.team_id < team_b.team_id;
}

bool operator== (const Team& team_a, const Team& team_b)
{
    return team_a.team_id == team_b.team_id;
}

bool operator!= (const Team& team_a, const Team& team_b)
{
    return !(team_a == team_b);
}

bool operator<= (const Team& team_a, const Team& team_b)
{
    return !(team_b < team_a);
}

bool operator>= (const Team& team_a, const Team& team_b)
{
    return team_b <= team_a;
}

bool operator> (const Team& team_a, const Team& team_b)
{
    return team_b < team_a;
}
int Team::getNumPlayers()
{
    return numPlayers;
}
int Team::getGamesPlayed()
{
    return gamesPlayed;
}
bool Team::get_is_valid()
{
    return is_valid;
}
void Team::set_team_insert(int xp_to_add , bool goalkeeprer , shared_ptr<PlayerByGoals> player_to_add)
{
    numPlayers++;
    xp+=xp_to_add;
    if(goalkeeprer == true)
    {
        numGoalkeeprs++;
    }
    if(numGoalkeeprs > 0 && numPlayers >= 11)
    {
        is_valid = true;
    }
    if(valid_team != nullptr)
    {
        valid_team->setxp(xp_to_add);
    }
    if(player_to_add->getGoals() == goals_highest_scorer)
    {
        if(player_to_add->getCards() == cards_highest_scorer)
        {
            if(player_to_add->getID() > id_highest_scorer)
            {
                id_highest_scorer = player_to_add->getID();
            }
        }
        else
        {
            if(player_to_add->getCards() < cards_highest_scorer)
            {
                id_highest_scorer = player_to_add->getID();
                cards_highest_scorer = player_to_add->getCards();
            }
        }
    }
    else
    {
        if(player_to_add->getGoals() > goals_highest_scorer)
        {
            id_highest_scorer = player_to_add->getID();
            cards_highest_scorer = player_to_add->getCards();
            goals_highest_scorer = player_to_add->getGoals();
        }
    }

}
void Team::set_team_remove(int xp_to_decrease , bool goalkeeprer , shared_ptr<PlayerByGoals> highest) {
    xp -= xp_to_decrease;
    numPlayers--;
    if (goalkeeprer == true)
    {
        numGoalkeeprs--;
    }
    if(numGoalkeeprs == 0 || numPlayers < 11)
    {
        is_valid = false;
    }
    if(valid_team != nullptr)
    {
        valid_team->setxp(-xp_to_decrease);
    }
    if(numPlayers == 0)
    {
        id_highest_scorer=0;
        cards_highest_scorer=0;
        goals_highest_scorer=0;
    }
    else
    {
        if(highest != nullptr)
        {
            id_highest_scorer = highest->getID();
            cards_highest_scorer = highest->getCards();
            goals_highest_scorer = highest->getGoals();
        }
    }

}

void Team::set_top_scorer(int id , int cards , int goals)
{
    id_highest_scorer = id;
    cards_highest_scorer = cards;
    goals_highest_scorer = goals;
}
shared_ptr<Tree<PlayerByID>> Team::get_team_players_by_id_tree()
{
    return team_players_by_id_tree;
}
shared_ptr<Tree<PlayerByGoals>> Team::get_team_players_by_goals_tree()
{
    return team_players_by_goals_tree;
}
int Team::get_id_highest_scorer()
{
    return id_highest_scorer;
}
void Team::set_gamesplayed(int new_games_played)
{
    gamesPlayed = new_games_played;
}
int Team::get_points() {
    return points ;
}
void Team::set_points(int new_points)
{
    points += new_points;
}
void Team::set_numPlayers(int num){
    numPlayers+= num;
}
void Team::set_numGoalkeepers(int num){
    numGoalkeeprs += num;
}
int Team::get_num_Goalkeepers(){
    return numGoalkeeprs;
}
void Team::set_invalid(bool b){
    is_valid = b;
}
int Team::get_cards_topscorer(){
    return cards_highest_scorer;
}
int Team::get_goals_topscorer() {
    return goals_highest_scorer;
}
void Team::set_by_id(shared_ptr<Tree<PlayerByID>> t)
{
    team_players_by_id_tree = t ;
}
void Team::set_by_goals(shared_ptr<Tree<PlayerByGoals>> t)
{
    team_players_by_goals_tree = t;
}
void Team::update_top_scorer(shared_ptr<PlayerByGoals> player){
    if(player->getGoals() == goals_highest_scorer)
    {
        if(player->getCards() == cards_highest_scorer)
        {
            if(player->getID() > id_highest_scorer)
            {
                id_highest_scorer = player->getID();

            }
        }
        else
        {
            if(player->getCards() < cards_highest_scorer)
            {
                id_highest_scorer = player->getID();
                cards_highest_scorer = player->getCards();
            }
        }
    }
    else
    {
        if(player->getGoals() > goals_highest_scorer)
        {
            id_highest_scorer = player->getID();
            cards_highest_scorer = player->getCards();
            goals_highest_scorer = player->getGoals();
        }
    }
}
void Team::changeTopscorerAfterRemove()
{
    std::shared_ptr<TreeNode<PlayerByGoals>> temp = team_players_by_goals_tree->getRoot();
    while(temp != nullptr)
    {
        if(temp->getRight() == nullptr)
        {
            id_highest_scorer = temp->getData()->getID();
            cards_highest_scorer =temp->getData()->getCards();
            goals_highest_scorer = temp->getData()->getGoals();
        }
        temp = temp->getRight();

    }
}