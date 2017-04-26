//Operating Systems Project #2 - Memory Management
//Trulee Hersh - hersht
//Mohammed Moftah - moftam
//Alex Seo - seoa

class Process{
	public:
		Process();

		//ACCESSORS
		char getPid() const;
		int getArrivalTime1() const;
		int getRunTime1() const;
		int getEndTime1() const;
		int getArrivalTime2() const;
		int getRunTime2() const;
		int getEndTime2() const;
		int getMemFrames() const;
		
		//MODIFIERS
		void setPid(char p);
		void setArrivalTime1(int a);
		void setRunTime1(int a);
		void setEndTime1(int a);
		void setArrivalTime2(int a);
		void setRunTime2(int a);
		void setEndTime2(int a);
		void setMemFrames(int a);
		
	private:
	    char pid;
	    int arrivalTime1;
	    int runTime1;
	    int arrivalTime2;
	    int runTime2;
	    int memFrames;
	    int endTime1;
	    int endTime2;
	    
};