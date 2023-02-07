void main()
{//Reset the current level if the timer ends
	void branch	= openborvariant("current_branch");
	int players	= openborvariant("count_players");
	float time	= openborvariant("game_time");
	float limit	= 0.5;

	//RESTART THE CURRENT LEVEL ONLY IF THERE'S ANY PLAYER ON THE SCREEN, OTHERWISE PROCEED TO THE NORMAL GAME OVER
	if(time <= limit && players > 0){jumptobranch(branch, 1);}
}