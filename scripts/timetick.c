void main()
{
	void branch	= openborvariant("current_branch");
	float time	= openborvariant("game_time");
	float limit	= 0.5;

	if(time <= limit){jumptobranch(branch, 1);}
}