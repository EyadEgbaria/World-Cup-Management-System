#include "worldcup23a1.h"

world_cup_t::world_cup_t():
        id_top_scorer(0),
        goals_of_highest_scorer(0),
        cards_of_highest_scorer(0),
        num_all_players(0),
        all_players_by_id_tree(new Tree<PlayerByID>),
        all_players_by_goals_tree(new Tree<PlayerByGoals>),
        all_teams(new Tree<Team>),
        valid_teams(new Tree<validTeams>)
{
    // TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
    shared_ptr<Team>* nodes = new shared_ptr<Team>[this->all_teams->getSize()];
    int index = 0;
    this->all_teams->inOrderDataToArray(all_teams->getRoot() , nodes , &index);
    for(int iter = 0; iter < this->all_teams->getSize() ; iter++ )
    {
        nodes[iter]->get_team_players_by_goals_tree()->destroyTree(nodes[iter]->get_team_players_by_goals_tree()->getRoot());
        nodes[iter]->get_team_players_by_id_tree()->destroyTree(nodes[iter]->get_team_players_by_id_tree()->getRoot());
    }
    delete[] nodes;
    this->all_teams->destroyTree(this->all_teams->getRoot());
    this->all_players_by_id_tree->destroyTree(this->all_players_by_id_tree->getRoot());
    this->all_players_by_goals_tree->destroyTree(this->all_players_by_goals_tree->getRoot());
    this->valid_teams->destroyTree(this->valid_teams->getRoot());
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    // TODO: Your code goes here
    if(teamId <= 0 || points < 0)///this null
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Team> team_to_add(new Team(teamId, points));
        shared_ptr<TreeNode<Team>> team_to_find(all_teams->find(team_to_add));
        if(team_to_find != nullptr){
            return StatusType::FAILURE;
        }
        //add team to all team tree
        all_teams->insert(team_to_add);
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::remove_team(int teamId)
{
    // TODO: Your code goes here
    if(teamId <= 0)
    {
        return StatusType:: INVALID_INPUT;
    }
    try {
        shared_ptr<Team> team_to_find(new Team(teamId, 0));
        shared_ptr<validTeams> valid_team_to_find(new validTeams(teamId, 0));
        shared_ptr<TreeNode<Team>> all_team_node(all_teams->find(team_to_find));
        shared_ptr<TreeNode<validTeams>> valid_team_node(valid_teams->find(valid_team_to_find));
        if(all_team_node == nullptr || all_team_node->getData()->getNumPlayers() > 0)
        {
            return StatusType::FAILURE;
        }
        all_teams->remove(team_to_find);
        if(valid_team_node != nullptr)
        {
            valid_teams->remove(valid_team_to_find);
        }
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    // TODO: Your code goes here
    if(playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0 || (gamesPlayed == 0 && (goals > 0 || cards > 0) ))
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<Team> team_to_find(new Team(teamId, 0));
        shared_ptr<TreeNode<Team>> all_team_node(all_teams->find(team_to_find));
        if(all_team_node == nullptr)
        {
            return StatusType::FAILURE;
        }
        shared_ptr<PlayerByID> player_id_to_add(new PlayerByID(playerId, all_team_node->getData()));//////
        shared_ptr<TreeNode<PlayerByID>> all_player_id_node(all_players_by_id_tree->find(player_id_to_add));
        if(all_player_id_node != nullptr)
        {
            return StatusType::FAILURE;
        }
        all_players_by_id_tree->insert(player_id_to_add);
        ////////
        shared_ptr<PlayerByGoals> player_goal_to_add(new PlayerByGoals(playerId , goals , cards , gamesPlayed-all_team_node->getData()->getGamesPlayed() , all_team_node->getData()->getGamesPlayed() , goalKeeper));
        all_players_by_goals_tree->insert(player_goal_to_add);
        player_id_to_add->setGoalsPtr(player_goal_to_add);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , player_goal_to_add);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , player_goal_to_add->getPrevious());
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , player_goal_to_add->getNext());

        all_team_node->getData()->set_team_insert(goals - cards , goalKeeper , player_goal_to_add);
        shared_ptr<validTeams> valid_team_to_find(new validTeams(teamId , all_team_node->getData()->getxp()));////
        shared_ptr<TreeNode<validTeams>> valid_team_node(valid_teams->find(valid_team_to_find));
        if(all_team_node->getData()->get_is_valid() && valid_team_node == nullptr)
        {
            valid_teams->insert(valid_team_to_find);
            all_team_node->getData()->setValidTeam(valid_team_to_find);//////

        }
        else
        {

            /*
            if(valid_team_node != nullptr)
            {   if(all_team_node->getData()->get_is_valid()){
                valid_teams->insert(valid_team_to_find);
                all_team_node->getData()->setValidTeam(valid_team_to_find);
            }
                valid_team_node->getData()->setxp(goals - cards);
            }
             */
        }
        all_team_node->getData()->get_team_players_by_id_tree()->insert(player_id_to_add);
        all_team_node->getData()->get_team_players_by_goals_tree()->insert(player_goal_to_add);

        update_top_scorer(player_goal_to_add);
        num_all_players++;
        return StatusType::SUCCESS;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::remove_player(int playerId)
{
    // TODO: Your code goes here
    if(playerId <= 0 )
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<PlayerByID> player_id_to_remove(new PlayerByID(playerId , nullptr));
        shared_ptr<TreeNode<PlayerByID>> all_player_id_node (all_players_by_id_tree->find(player_id_to_remove));
        if(all_player_id_node == nullptr)
        {
            player_id_to_remove= nullptr;
            return StatusType::FAILURE;
        }
        shared_ptr<Team> team_node(all_player_id_node->getData()->getTeamPtr());
        shared_ptr<PlayerByGoals> player_goal_to_remove(all_player_id_node->getData()->getGoalsPtr());
        shared_ptr<validTeams> valid_team_node = team_node->getValidTeam();



        if(team_node->get_id_highest_scorer() == player_goal_to_remove->getID())
        {
            team_node->set_team_remove(player_goal_to_remove->getGoals() - player_goal_to_remove->getCards() , player_goal_to_remove->getGoalkeeperFlag() , player_goal_to_remove->getPrevious());
        }
        else
        {
            team_node->set_team_remove(player_goal_to_remove->getGoals() - player_goal_to_remove->getCards() , player_goal_to_remove->getGoalkeeperFlag() , nullptr);
        }
        if(valid_team_node != nullptr && team_node->get_is_valid() == false)
        {
            team_node->setValidTeam(nullptr);
            valid_teams->remove(valid_team_node);
        }
        else
        {
            /*if(team_node->get_is_valid() == true) {
                valid_team_node->setxp(player_goal_to_remove->getCards() - player_goal_to_remove->getGoals());/////////////////
            }*/
        }
        team_node->get_team_players_by_goals_tree()->remove(player_goal_to_remove);
        team_node->get_team_players_by_id_tree()->remove(player_id_to_remove);
        team_node->changeTopscorerAfterRemove();

        /*if(team_node->get_id_highest_scorer() == player_goal_to_remove->getID())
        {
            change_highest_scorer_before_remove_in_the_team(team_node , player_goal_to_remove);
        }*/
        if(num_all_players == 1 )
        {
            id_top_scorer = 0;
            goals_of_highest_scorer = 0 ;
            cards_of_highest_scorer = 0 ;
            num_all_players = 0 ;
            player_goal_to_remove->setPrevious(nullptr);
            player_goal_to_remove->setNext(nullptr);
            all_player_id_node->getData()->setTeamPtr(nullptr);
            all_player_id_node->getData()->setGoalsPtr(nullptr);
            all_players_by_goals_tree->remove(player_goal_to_remove);
            all_players_by_id_tree->remove(player_id_to_remove);



            /// UPDATE TOP SCORER TO NULL OR SOMESHIT



            return StatusType::SUCCESS;
        }
        if(id_top_scorer == player_goal_to_remove->getID())
        {
            change_highest_scorer_before_remove(player_goal_to_remove);
        }
        //check ***********
        shared_ptr<TreeNode<PlayerByGoals>> realPlayer(all_players_by_goals_tree->find(player_goal_to_remove));
        shared_ptr<PlayerByGoals> prev_to_change = realPlayer->getData()->getPrevious();
        shared_ptr<PlayerByGoals> next_to_change = realPlayer->getData()->getNext();
        player_goal_to_remove->setPrevious(nullptr);
        player_goal_to_remove->setNext(nullptr);
        all_player_id_node->getData()->setTeamPtr(nullptr);
        all_player_id_node->getData()->setGoalsPtr(nullptr);
        all_players_by_goals_tree->remove(player_goal_to_remove);
        all_players_by_id_tree->remove(player_id_to_remove);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , prev_to_change);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , next_to_change);
        num_all_players--;
        player_id_to_remove= nullptr;
    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    /// SET TOP SCORER OF THE TEAM TO MAXIMUM RIGHT


    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                            int scoredGoals, int cardsReceived)
{
    // TODO: Your code goes here
    if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived <0)
    {
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<PlayerByID> player_id_to_find(new PlayerByID(playerId , nullptr));
        shared_ptr<TreeNode<PlayerByID>> player = all_players_by_id_tree->find(player_id_to_find);
        if(player == nullptr)
        {
            return StatusType::FAILURE;
        }
        //temp variables to save detailes about player
        int player_id , goals , cards , gamesPlayed2 , teamGames ;
        bool goalKeeperFlag;
        player_id = player->getData()->getID();
        goals = player->getData()->getGoalsPtr()->getGoals();
        cards = player->getData()->getGoalsPtr()->getCards();
        gamesPlayed2 = player->getData()->getGoalsPtr()->getGamesPlayed();
        teamGames = player->getData()->getGoalsPtr()->getGamesTeam();
        goalKeeperFlag = player->getData()->getGoalsPtr()->getGoalkeeperFlag();
        shared_ptr<Team> player_team = player->getData()->getTeamPtr();
        shared_ptr<PlayerByGoals> player_goal = player->getData()->getGoalsPtr();
        shared_ptr<PlayerByGoals> prev_player = player_goal->getPrevious();
        shared_ptr<PlayerByGoals> next_player = player_goal->getNext();
        player_goal->setPrevious(nullptr);
        player_goal->setNext(nullptr);


        //remove player from goals trees to update his stats
        all_players_by_goals_tree->remove(player_goal);
        player_team->get_team_players_by_goals_tree()->remove(player_goal);

        //created updated player
        shared_ptr<PlayerByGoals> new_player_by_goal(new PlayerByGoals(player_id , goals + scoredGoals , cards + cardsReceived , gamesPlayed2 + gamesPlayed , teamGames , goalKeeperFlag));
        player->getData()->setGoalsPtr(new_player_by_goal);

        //insert updated player to goal trees and chack if is heighst scorer
        all_players_by_goals_tree->insert(new_player_by_goal);
        update_top_scorer(new_player_by_goal);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , new_player_by_goal);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , prev_player);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , next_player);
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , new_player_by_goal->getPrevious());
        updatePreAndSuc(all_players_by_goals_tree->getRoot() , new_player_by_goal->getNext());
        player_team->get_team_players_by_goals_tree()->insert(new_player_by_goal);
        player_team->update_top_scorer(new_player_by_goal);

        //update the teams
        player_team->setxp(player_team->getxp() + scoredGoals - cardsReceived);
        if(player_team->getValidTeam() != nullptr)
        {
            player_team->getValidTeam()->setxp(scoredGoals - cardsReceived);
        }

    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if( teamId2 <= 0 || teamId1 <= 0 || teamId2 == teamId1){
        return StatusType::INVALID_INPUT;
    }

    try {
        shared_ptr<Team> team_to_find (new Team(teamId1,0));
        shared_ptr<TreeNode<Team>> team1 (all_teams->find(team_to_find));
        shared_ptr<Team> team_to_find2 (new Team(teamId2,0));
        shared_ptr<TreeNode<Team>> team2 (all_teams->find(team_to_find2));
        if(team1 == nullptr || team2 == nullptr){
            return StatusType::FAILURE;
        }
        bool t1 = team1->getData()->get_is_valid();
        bool t2 = team2->getData()->get_is_valid();
        if(!t1 || !t2){
            return StatusType::FAILURE;
        }
        int prev_xp1 = team1->getData()->getxp();
        int prev_xp2 = team2->getData()->getxp();
        if(team1->getData()->getxp() > team2->getData()->getxp()){
            team1->getData()->setxp(prev_xp1 + 3);
            team2->getData()->setxp(prev_xp2 + 0);
            team2->getData()->set_points( 0);
            team1->getData()->set_points( 3);

            team1->getData()->getValidTeam()->setxp(3);
            team2->getData()->getValidTeam()->setxp(0);
        }
        if(team2->getData()->getxp() > team1->getData()->getxp()){
            team2->getData()->setxp(prev_xp2 + 3);
            team1->getData()->setxp(prev_xp1 + 0);

            team2->getData()->getValidTeam()->setxp(3);
            team1->getData()->getValidTeam()->setxp(0);
            team2->getData()->set_points( 3);
            team1->getData()->set_points( 0);
        }
        if(team2->getData()->getxp() == team1->getData()->getxp()){
            team1->getData()->setxp(prev_xp1 + 1);
            team2->getData()->setxp(prev_xp2 + 1);

            team1->getData()->getValidTeam()->setxp(1);
            team2->getData()->getValidTeam()->setxp(1);
            team2->getData()->set_points( 1);
            team1->getData()->set_points( 1);
        }
        team1->getData()->set_gamesplayed(team1->getData()->getGamesPlayed()+1);
        team2->getData()->set_gamesplayed(team2->getData()->getGamesPlayed()+1);
        return StatusType::SUCCESS;

    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    // TODO: Your code goes here
    if(playerId <= 0 )
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<PlayerByID> player_to_find(new PlayerByID(playerId , nullptr));
        shared_ptr<TreeNode<PlayerByID>> all_player_node(all_players_by_id_tree->find(player_to_find));
        if(all_player_node == nullptr)
        {
            return StatusType::FAILURE;
        }
        shared_ptr<PlayerByGoals> all_player_goal = all_player_node->getData()->getGoalsPtr();
        int num = all_player_goal->getGamesPlayed() + all_player_node->getData()->getTeamPtr()->getGamesPlayed();
        //int num = all_player_goal->getGamesPlayed() + all_player_node->getData()->getTeamPtr()->getGamesPlayed()-all_player_goal->getGamesTeam();
        return num;
    }
    catch(std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
        try{
            shared_ptr<Team> team_to_find (new Team(teamId,0));
            shared_ptr<TreeNode<Team>> team (all_teams->find(team_to_find));
            if(team == nullptr){
                return StatusType::FAILURE;
            }

            return team->getData()->get_points();
        }
        catch(std::bad_alloc& ba)
        {
            return StatusType::ALLOCATION_ERROR;
        }

}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    if(teamId1 == teamId2 || teamId2 <=0 || newTeamId <= 0 || teamId1 <= 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        shared_ptr<Team> team_to_find1 (new Team(teamId1,0));
        shared_ptr<TreeNode<Team>> team1 (all_teams->find(team_to_find1));
        shared_ptr<Team> team_to_find2 (new Team(teamId2,0));
        shared_ptr<TreeNode<Team>> team2 (all_teams->find(team_to_find2));
        shared_ptr<Team> team_to_find_new (new Team(newTeamId,0));
        shared_ptr<TreeNode<Team>>new_team (all_teams->find(team_to_find_new));

        if(team1 == nullptr || team2 == nullptr ){
            return StatusType::FAILURE;
        }
        if(new_team != nullptr && newTeamId != teamId1 && newTeamId != teamId2){
            return StatusType::FAILURE;
        }
        shared_ptr<Team> team_to_add (new Team(newTeamId , team1->getData()->get_points() + team2->getData()->get_points()));
        team_to_add->setxp(team1->getData()->getxp() + team2->getData()->getxp());
        team_to_add->set_numPlayers(team1->getData()->getNumPlayers() + team2->getData()->getNumPlayers());
        team_to_add->set_numGoalkeepers(team1->getData()->get_num_Goalkeepers() +team2->getData()->get_num_Goalkeepers());
        int new_num_players = team1->getData()->getNumPlayers() + team2->getData()->getNumPlayers();
        if(team_to_add->getNumPlayers() >= 11 && team_to_add->get_num_Goalkeepers() >=1)
        {
            team_to_add->set_invalid(true);
        }
        int id_top_scorer1 =team1->getData()->get_id_highest_scorer();
        int goals_top_scorer1 =team1->getData()->get_goals_topscorer();
        int cards_top_scorer1 =team1->getData()->get_cards_topscorer();
        int id_top_scorer2 =team2->getData()->get_id_highest_scorer();
        int goals_top_scorer2 =team2->getData()->get_goals_topscorer();
        int cards_top_scorer2 =team2->getData()->get_cards_topscorer();
        int new_topscorer = max_top_scorer(id_top_scorer1,goals_top_scorer1,cards_top_scorer1,id_top_scorer2,goals_top_scorer2,cards_top_scorer2);
        if(new_topscorer == id_top_scorer1) {
            team_to_add->set_top_scorer(id_top_scorer1, cards_top_scorer1, goals_top_scorer1);
        }
        else{
            team_to_add->set_top_scorer(id_top_scorer2, cards_top_scorer2, goals_top_scorer2);
        }
        shared_ptr<Tree<PlayerByGoals>> goals_merge(new Tree<PlayerByGoals>);
        shared_ptr<Tree<PlayerByID>> id_merge(new Tree<PlayerByID>);
        goals_merge = mergeTrees(team1->getData()->get_team_players_by_goals_tree() ,team2->getData()->get_team_players_by_goals_tree() );
        id_merge = mergeTrees(team1->getData()->get_team_players_by_id_tree() ,team2->getData()->get_team_players_by_id_tree());

        int index = 0 ; //do deleteeee*******....................................
        shared_ptr<PlayerByID>* merge_array = new shared_ptr<PlayerByID>[new_num_players];
        id_merge->inOrderDataToArray(id_merge->getRoot() , merge_array , &index);
        for(int i = 0 ; i < new_num_players ; i++)
        {
            //int games_late = merge_array[i]->getGoalsPtr()->getGamesTeam();
            int games_player = merge_array[i]->getGoalsPtr()->getGamesPlayed();
            int team_games = merge_array[i]->getTeamPtr()->getGamesPlayed();
            merge_array[i]->getGoalsPtr()->set_games_played(games_player + team_games);
            merge_array[i]->getGoalsPtr()->set_games_team(0);
            merge_array[i]->setTeamPtr(team_to_add);
        }
        delete[] merge_array;


        team_to_add->set_by_goals(goals_merge);
        team_to_add->set_by_id(id_merge);

        shared_ptr<Team> team1_to_remove(new Team(teamId1 , 0 ));
        shared_ptr<Team> team2_to_remove( new Team(teamId2 , 0));
        shared_ptr<validTeams> valid1_to_remove(new validTeams(teamId1 , 0));
        shared_ptr<validTeams> valid2_to_remove(new validTeams(teamId2 , 0));
        shared_ptr<TreeNode<validTeams>> valid1_node (valid_teams->find(valid1_to_remove));
        shared_ptr<TreeNode<validTeams>> valid2_node (valid_teams->find(valid2_to_remove));
        if(valid1_node != nullptr)
        {
            valid_teams->remove(valid1_to_remove);
        }
        if(valid2_node != nullptr)
        {
            valid_teams->remove(valid2_to_remove);
        }
        if(team_to_add->get_is_valid() == true)
        {
            shared_ptr<validTeams> valid_to_add(new validTeams(newTeamId ,team_to_add->getxp()));
            valid_teams->insert(valid_to_add);
            team_to_add->setValidTeam(valid_to_add);
        }
        all_teams->remove(team1_to_remove);
        all_teams->remove(team2_to_remove);
        all_teams->insert(team_to_add);



    }
    catch(std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{       if(teamId == 0){
        return StatusType::INVALID_INPUT;
        }
        if(teamId >0){
            try{
                shared_ptr<Team> team_to_find (new Team(teamId,0));
                shared_ptr<TreeNode<Team>> team (all_teams->find(team_to_find));
                if(team == nullptr){
                    return StatusType::FAILURE;
                }
                if(team->getData()->getNumPlayers() == 0){
                    return StatusType::FAILURE;
                }
                return team->getData()->get_id_highest_scorer();
            }
            catch(std::bad_alloc& ba)
            {
                return StatusType::ALLOCATION_ERROR;
            }
        }
        else{
                if(num_all_players == 0){
                    return StatusType::FAILURE;
                }
                return id_top_scorer;
        }

}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    if(teamId > 0){
        try{
            shared_ptr<Team> team_to_find (new Team(teamId,0));
            shared_ptr<TreeNode<Team>> team (all_teams->find(team_to_find));
            if(team == nullptr){
                return StatusType::FAILURE;
            }
            return team->getData()->getNumPlayers();
        }
        catch(std::bad_alloc& ba)
        {
            return StatusType::ALLOCATION_ERROR;
        }
    }
    else{
        return num_all_players;
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(teamId == 0 || output == nullptr)
    {
        return  StatusType::INVALID_INPUT;
    }
    try
    {
        int size = 0;
        shared_ptr<PlayerByGoals>* tree_array;
        if(teamId <= 0)
        {
            if(num_all_players == 0)
            {
                return StatusType::FAILURE;
            }
            size = num_all_players;
            tree_array = new shared_ptr<PlayerByGoals>[size];
            int index = 0 ;
            all_players_by_goals_tree->inOrderDataToArray(all_players_by_goals_tree->getRoot() , tree_array , &index);
        }
        else
        {
            shared_ptr<Team> team_to_find (new Team(teamId , 0));
            shared_ptr<TreeNode<Team>> real_team = all_teams->find(team_to_find);
            if(real_team == nullptr)
            {
                return StatusType::FAILURE;
            }
            if(real_team->getData()->getNumPlayers() == 0)
            {
                return StatusType::FAILURE;
            }
            size = real_team->getData()->getNumPlayers();
            tree_array = new shared_ptr<PlayerByGoals>[size];
            int index = 0 ;
            real_team->getData()->get_team_players_by_goals_tree()->inOrderDataToArray(real_team->getData()->get_team_players_by_goals_tree()->getRoot() , tree_array , &index);
        }
        for(int i=0 ; i < size ; i++)
        {
            output[i] = tree_array[i]->getID();
        }
        delete[] tree_array;

    }
    catch(std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if(playerId <= 0 || teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    try {
        shared_ptr<Team> team_to_find(new Team(teamId, 0));
        shared_ptr<TreeNode<Team>> team(all_teams->find(team_to_find));
        if(team == nullptr){
            return StatusType::FAILURE;
        }
        if(num_all_players == 1){
            return StatusType::FAILURE;
        }
        shared_ptr<PlayerByID> player_to_find(new PlayerByID(playerId ,nullptr ));
        shared_ptr<TreeNode<PlayerByID>> player = team->getData()->get_team_players_by_id_tree()->find(player_to_find);
        if(player == nullptr){
            return StatusType::FAILURE;
        }
        shared_ptr<PlayerByGoals> player_p = player->getData()->getGoalsPtr();
        shared_ptr<PlayerByGoals> previous = player_p->getPrevious();
        shared_ptr<PlayerByGoals> next = player_p->getNext();
        return best_player(previous,next,player_p);


    }
    catch(std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }

}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    if(minTeamId < 0 || maxTeamId < 0  || maxTeamId < minTeamId){
        return StatusType::INVALID_INPUT ;
    }
    try{

        shared_ptr<validTeams> min_team(new validTeams(minTeamId , 0));
        shared_ptr<validTeams> max_team(new validTeams(maxTeamId , 0));

        shared_ptr<validTeams>* data_array = new shared_ptr<validTeams>[valid_teams->getSize()];
        int index = 0 ;
        valid_teams->minMaxInOrderDataToArray(valid_teams->getRoot() , data_array , &index , min_team , max_team);
        if(index == 0)
        {
            return StatusType::FAILURE;
        }
        int* id_array = new int[index];
        int* xp_array = new int[index];
        for(int i = 0 ; i < index ; i++)
        {
            id_array[i] = data_array[i]->getID();
            xp_array[i] = data_array[i]->getXp();
        }
        delete[] data_array;
        int num = knockout_aux(id_array,xp_array,index);
        delete[] id_array;
        delete[] xp_array;
        return num;

    }
    catch (std::bad_alloc& ba)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

void world_cup_t::update_top_scorer(shared_ptr<PlayerByGoals> player)
{
    if(player->getGoals() == goals_of_highest_scorer)
    {
        if(player->getCards() == cards_of_highest_scorer)
        {
            if(player->getID() > id_top_scorer)
            {
                id_top_scorer = player->getID();

            }
        }
        else
        {
            if(player->getCards() < cards_of_highest_scorer)
            {
                id_top_scorer = player->getID();
                cards_of_highest_scorer = player->getCards();
            }
        }
    }
    else
    {
        if(player->getGoals() > goals_of_highest_scorer)
        {
            id_top_scorer = player->getID();
            cards_of_highest_scorer = player->getCards();
            goals_of_highest_scorer = player->getGoals();
        }
    }
}
void world_cup_t::change_highest_scorer_before_remove(shared_ptr<PlayerByGoals> player)
{
    shared_ptr<TreeNode<PlayerByGoals>> player_goal = all_players_by_goals_tree->find(player);
    if(player_goal->getLeft() != nullptr)
    {
        shared_ptr<PlayerByGoals> next_highest_player = player_goal->getLeft()->getData();
        id_top_scorer = next_highest_player->getID();
        goals_of_highest_scorer = next_highest_player->getGoals();
        cards_of_highest_scorer = next_highest_player->getCards();
    }
    else {
        shared_ptr<PlayerByGoals> next_highest_player = player_goal->getFather()->getData();
        id_top_scorer = next_highest_player->getID();
        goals_of_highest_scorer = next_highest_player->getGoals();
        cards_of_highest_scorer= next_highest_player->getCards();
    }
}

void world_cup_t::change_highest_scorer_before_remove_in_the_team(shared_ptr<Team> team_node , shared_ptr<PlayerByGoals> player)
{
    shared_ptr<TreeNode<PlayerByGoals>> player_goal = team_node->get_team_players_by_goals_tree()->find(player);
    if(player_goal == nullptr){
        return;
    }
    if(player_goal->getLeft() != nullptr)
    {
        shared_ptr<PlayerByGoals> next_highest_player = player_goal->getLeft()->getData();
        team_node->set_top_scorer(next_highest_player->getID() , next_highest_player->getCards() , next_highest_player->getGoals());
    }
    else {
        shared_ptr<PlayerByGoals> next_highest_player = player_goal->getFather()->getData();
        team_node->set_top_scorer(next_highest_player->getID() , next_highest_player->getCards() , next_highest_player->getGoals());
    }
}

void world_cup_t::updatePreAndSuc(shared_ptr<TreeNode<PlayerByGoals>> root  , shared_ptr<PlayerByGoals> key)
{
    if(root == nullptr || key == nullptr)
    {
        return;
    }
/*
    if(*(root->getData()) == *key)
    {
        if(root->getLeft() != nullptr)
        {
            shared_ptr<TreeNode<PlayerByGoals>> tmp = root->getLeft();
            while(tmp->getRight() != nullptr)
            {
                tmp = tmp->getRight();
            }
            key->setPrevious(tmp->getData());
        }

        if(root->getRight() != nullptr)
        {
            shared_ptr<TreeNode<PlayerByGoals>> tmp = root->getRight();
            while(tmp->getLeft() != nullptr)
            {
                tmp = tmp->getLeft();
            }
            key->setNext(tmp->getData());
        }

        return;
    }
    if(*(root->getData()) > *key)
    {
        key->setNext(root->getData());
        updatePreAndSuc(root->getLeft()  , key);
    }
    else
    {
        key->setPrevious(root->getData());
        updatePreAndSuc(root->getRight()  , key);
    }
*/

    shared_ptr<TreeNode<PlayerByGoals>> wanted = all_players_by_goals_tree->find(key);

    if (wanted == nullptr)
    {
        return;
    }

    shared_ptr<TreeNode<PlayerByGoals>> current = wanted;
    shared_ptr<TreeNode<PlayerByGoals>> currentParent = wanted->getFather();
    if (current->getLeft() != nullptr)
    {
        current = current->getLeft();
        while (current->getRight() != nullptr)
        {
            current = current->getRight();
        }
        if (current == nullptr)
            wanted->getData()->setPrevious(nullptr);
        else
            wanted->getData()->setPrevious(current->getData());
    }
    else
    {
        while (currentParent != nullptr && currentParent->getLeft() == current)
        {
            current = currentParent;
            currentParent = currentParent->getFather();
        }
        if (currentParent == nullptr)
            wanted->getData()->setPrevious(nullptr);
        else
            wanted->getData()->setPrevious(currentParent->getData());
    }
    current = wanted;
    currentParent = wanted->getFather();
    if (current->getRight() != nullptr)
    {
        current = current->getRight();
        while (current->getLeft() != nullptr)
        {
            current = current->getLeft();
        }
        if (current == nullptr)
            wanted->getData()->setNext(nullptr);
        else
            wanted->getData()->setNext(current->getData());
    }
    else
    {
        while (currentParent != nullptr && currentParent->getRight() == current)
        {
            current = currentParent;
            currentParent = currentParent->getFather();
        }
        if (currentParent == nullptr)
            wanted->getData()->setNext(nullptr);
        else
            wanted->getData()->setNext(currentParent->getData());
    }

}
int world_cup_t::max_top_scorer(int id1 , int goal1 , int cards1 , int id2 , int goal2 , int cards2)
{
    if(goal1 == goal2)
    {
        if(cards1 == cards2)
        {
            if(id1 > id2)
            {
                return id1;
            }
            return id2;
        }
        else
        {
            if(cards1< cards2)
            {
                return id1;
            }
            return id2;
        }
    }
    else{
        if(goal1 > goal2)
        {
            return id1;
        }
        return id2;
    }
}
int world_cup_t::best_player(shared_ptr<PlayerByGoals> p1 ,shared_ptr<PlayerByGoals> p2 ,shared_ptr<PlayerByGoals> current ) {
    if (p1 == nullptr) {
        return p2->getID();
    }
    if (p2 == nullptr) {
        return p1->getID();
    }
    if (p2->getGoals() - current->getGoals() > current->getGoals() - p1->getGoals()) {
        return p1->getID();
    }
    if (p2->getGoals() - current->getGoals() < current->getGoals() - p1->getGoals()) {
        return p2->getID();
    }
    //cards
    int p1_defference = p1->getCards() - current->getCards();
    int p2_defference = current->getCards() - p2->getCards();
    if (p1_defference < 0) {
        p1_defference *= -1;
    }
    if (p2_defference < 0) {
        p2_defference *= -1;
    }
    if (p1_defference < p2_defference) {
        return p1->getID();
    }
    if (p1_defference > p2_defference) {
        return p2->getID();
    }
    //id
    int id1_def = p1->getID() - current->getID();
    int id2_def = p2->getID() - current->getID();
    if (id1_def < 0) {
        id1_def *= -1;
    }
    if (id2_def < 0) {
        id2_def *= -1;
    }
    if (id1_def > id2_def) {
        return p2->getID();
    }
    if (id1_def < id2_def) {
        return p1->getID();
    }
    if (p1->getID() > p2->getID()) {
        return p1->getID();
    }

    return p2->getID();

}

int world_cup_t::knockout_aux(int* id_array , int* xp_array , int length )
{
    /*
    if(length == 1)
    {
        return id_array[0];
    }
    int num ;
    int* id_array_aux;
    int* xp_array_aux;
    if(length % 2 == 0){
        id_array_aux = new int[length/2];
        xp_array_aux = new int[length/2];
        for(int i = 0 ; i < length ; i+=2){
            if(xp_array[i] > xp_array[i+1]){
                id_array_aux[i]=id_array[i];
                xp_array_aux[i] = xp_array[i]+xp_array[i+1]+3 ;
            }
            if(xp_array[i] <= xp_array[i+1]){
                id_array_aux[i]=id_array[i+1];
                xp_array_aux[i] = xp_array[i]+xp_array[i+1]+3 ;
            }

        }
        num = knockout_aux(id_array_aux , xp_array_aux ,length/2);
    }
    else
    {
        id_array_aux = new int[length/2 +1];
        xp_array_aux = new int[length/2 +1];
        int i =0 ;
        for(; i < length-1 ; i+=2){
            if(xp_array[i] > xp_array[i+1]){
                id_array_aux[i]=id_array[i];
                xp_array_aux[i] = xp_array[i]+xp_array[i+1]+3 ;
            }
            if(xp_array[i] <= xp_array[i+1]){
                id_array_aux[i]=id_array[i+1];
                xp_array_aux[i] = xp_array[i]+xp_array[i+1]+3 ;
            }
        }
        id_array_aux[length/2] = id_array[length-1];
        xp_array_aux[length/2] = xp_array[length-1];
        num = knockout_aux(id_array_aux , xp_array_aux ,length/2 +1);

    }
    delete[] id_array_aux;
    delete[] xp_array_aux;
    return num;
     */
   /* int counter = 0 ;
    while(length != 1)
    {
        counter = 0 ;
        for(int i= 0 ; i < length ; i+=2)
        {
            if(xp_array[i] <= xp_array[i+1])
            {
                id_array[counter] = id_array[i+1];
                xp_array[counter] = xp_array[i] + xp_array[i+1]+3;
            }
            else{
                id_array[counter] = id_array[i];
                xp_array[counter] = xp_array[i] + xp_array[i+1]+3;
            }
            counter++;
            if(i+1 == length-1)
            {
                id_array[counter] = id_array[i+1];
                xp_array[counter] = xp_array[i+1];
            }
        }
        length = counter;
    }
    int id_winner = id_array[length-1];
    return id_winner;
*/
    int i = 0 , jump2 = 2 , jump1 = 1 ;
    while (jump1<=length) {
        i=0;
        for (; i + jump1 < length; i += jump2) {
            if (xp_array[i] < xp_array[i + jump1] ||
                (xp_array[i] == xp_array[i + jump1] && id_array[i] < id_array[i + jump1])) {
                id_array[i]=id_array[i+jump1];

            }
            xp_array[i]+= 3 + xp_array[i+jump1];
        }
        jump1*=2;
        jump2*=2;
    }
    return id_array[0];
}

