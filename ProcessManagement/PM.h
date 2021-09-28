#using <System.dll>
using namespace System;
ref class Program {
	private:
		bool crit;
		int count;
		int countLimit;
		String^ programName;

	public:
		Program(String^,bool,int);
		int handelHeartBeat();
		void setCount();
		String^ getName();
		~Program();
};
