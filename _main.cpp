#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <thread>
#include <windows.h>

#pragma warning(disable:4996)
// localtime때문
using namespace std;

enum cas {
	quit,
	studylist,
	study,
	stop,
	e_timeline,
	save,
	temp,
	resume,
	
};

class f_time {
public:
	int f_hour, f_min, f_sec;
	void f_time1(int h, int m, int s) {
		f_hour = h;
		f_min = m;
		f_sec = s;
	}
};

class psscc : public f_time {
	string whatstudy;
	vector<pair<string, int> > c_timeline;
	time_t tptm;     //시간계산용
	tm *temp_tm;     //시간계산용
	f_time timetemp; //시간계산용
	vector<pair<string, pair<int, bool>> > c_studylist; // class_studylist
	bool timestop = false; // 2번의 중단과 재개를 위함
	int cnt;
	static void clearInputBuffer()
	{
		// 입력 버퍼에서 문자를 계속 꺼내고 \n를 꺼내면 반복을 중단
		while (getchar() != '\n');
	}
	static void clearInputfileBuffer(ifstream& ifs, int cnt) { // cnt 라인지우는횟수
		string temp;
		while (cnt--) {
			getline(ifs, temp);
		}
	}
public:
	psscc(string s) : tptm(time(NULL)), temp_tm(localtime(&tptm)), whatstudy(s) {
		f_time1(temp_tm->tm_hour, temp_tm->tm_min, temp_tm->tm_sec);
		input_studylist();
	}
	void push_reason1(string s, int n = 0) { 
		ostringstream oss;
		int thour = temp_tm->tm_hour - f_hour;
		int tmin = temp_tm->tm_min - f_min;
		if (tmin < 0) {
			tmin += 60;
			thour--;
		}
		if (thour < 0) {
			thour += 24;
		}
		int tsec = temp_tm->tm_sec - f_sec;
		oss << thour << "시 " << tmin << "분 " << tsec << "초  :   ";
		s = oss.str() + s;
		c_timeline.push_back(make_pair(s, n));
	}

	void print_main(void)
	{
		//시분초가 2자리수가 아니면 오른쪽 막는 글자가 땡겨져온다.
		string h, m, s;
		h.clear();
		m.clear();
		s.clear();
		if (f_hour < 10) {
			h += ' ';
		}
		h += to_string(f_hour);
		if (f_min < 10) {
			m += ' ';
		}
		m += to_string(f_min);
		if (f_sec < 10) {
			s += ' ';
		}
		s += to_string(f_sec);


		system("cls");
		cout << "        ┌───────────────────────────────┐" << endl;
		cout << "        │ 공부 시작시간 : ";
		cout << h << "시 " << m << "분 " << s << "초" << "│" <<  endl;
		cout << "        │ 1. 공부할 것 리스트           │" << endl;
		cout << "        │ 2. 공부한 것 타임라인 추가    │" << endl;
		cout << "        │ " << (timestop ? "3. 재개" : "3. 중단")  << "                       │ "<< endl;
		cout << "        │ 4. 타임라인                   │" << endl;
		cout << "        │ 5. 저장                       │" << endl;
		cout << "        │ 0. 종료                       │" << endl;
		cout << "        └───────────────────────────────┘" << endl;
		cout << "                 입력하세요 : ";
	}

	bool menu(void) {
		print_main();
		int select;
		if (!(cin >> select)) {
			cin.clear();
			clearInputBuffer();
			select = 99;
		};

		if (timestop && select == stop) {
			select = resume;
		}

		cal_temp_time();
		system("cls");

		//for switch case문안에서 변수선언 못하기 때문에 밖에다 만듬
		int n_forresume = 0;
		string temp;
		int _h, _m; // in resume
					//---------------------------------------------------
		switch (select) {
		case quit:
			return false;
		case studylist:
			print_fix_studylist();
			system("pause");
			break;
		case study:
			cout << "공부한 것 한문장으로 입력하세요 : ";
			clearInputBuffer();
			getline(cin, temp);
			temp = "공부  " + temp;
			push_reason1(temp);
			break;
		case stop:
			timetemp.f_time1(temp_tm->tm_hour, temp_tm->tm_min, temp_tm->tm_sec);
			timestop = true;
			break;
		case resume:
			cout << "중단 이유를 입력하세요 : ";
			clearInputBuffer();
			getline(cin, temp);
			_h = temp_tm->tm_hour - timetemp.f_hour;
			if (_h < 0) {
				_h += 24;
			}
			n_forresume = (_h) * 60;
			_m = temp_tm->tm_min - timetemp.f_min;
			if (_m < 0) {
				_h--;
				_m += 60;
			}
			n_forresume += temp_tm->tm_min - timetemp.f_min;
			temp = "중단  " + temp;
			push_reason1(temp, n_forresume);
			timestop = false;
			break;
		case e_timeline:
			print_timeline();
			system("pause");
			break;
		case save:
			handsave();
			cout << "save 완료" << endl;
			system("pause");
			break;
		default:
			cout << "잘못된 입력" << endl;
			system("pause");
			break;
		}
		return true;
	}
	void input_studylist(void) {
		ifstream studylist_input("list.txt");
		int studylist_num;
		studylist_input >> studylist_num;

		clearInputfileBuffer(studylist_input, 2);

		for (int i = 0; i < studylist_num; i++) {
			string temp;
			studylist_input >> temp; // 쓰레기값 지우기
			getline(studylist_input, temp);
			c_studylist.push_back(make_pair(temp, make_pair(i + 1, false)));
		}
	}
	void print_timeline(void) {
		cout << "---타임라인---" << endl;
		int len = c_timeline.size();
		for (int i = 0; i < len; ++i) {
			cout << i + 1 << ".  " << c_timeline[i].first << "  소모시간 : " << c_timeline[i].second << "분" << endl;
		}
		cout << endl;
	}
	void print_fix_studylist(void) { // 출력하고 OX수정까지
		cout << "공부할 목록" << endl;
		int len = c_studylist.size();
		for (int i = 0; i < len; ++i) {
			cout << c_studylist[i].second.first << ". : " << c_studylist[i].first <<
				"   했나요? : " << (c_studylist[i].second.second ? "O" : "X") << endl;
		}
		cout << endl;
		cout << "했는지 수정할 것 번호를 입력 or 돌아가기는 0번 : ";
		int select = -1; // while일단 돌아가도록

		while (!fix(select))
		{
			system("cls");
			cout << "공부할 목록" << endl;
			int len = c_studylist.size();
			for (int i = 0; i < len; ++i) {
				cout << c_studylist[i].second.first << ". : " << c_studylist[i].first <<
					"   했나요? : " << (c_studylist[i].second.second ? "O" : "X") << endl;
			}
			cout << endl;
			cout << "했는지 수정할 것 번호를 입력 or 돌아가기는 0번 : ";
			cin >> select;
		}; // 고치거나 돌아가는 함수
	}

	int fix(int select) {
		if (0 > select || select > c_studylist.size()) {
			return false;
		}
		if (select == 0) {
			return true;
		}
		c_studylist[select - 1].second.second = !c_studylist[select - 1].second.second;
		return false;
	}

	void cal_temp_time(void) {
		tptm = time(NULL);
		temp_tm = localtime(&tptm);
	}

	ostream& study_feedback(ostream& ofs) {
		cal_temp_time();
		ofs << temp_tm->tm_year + 1900 << "년 " << temp_tm->tm_mon + 1 << "월 " << temp_tm->tm_mday << "일 ";
		ofs << '\"' << whatstudy << '\"' << "에 대한 공부" << endl;
		ofs << "공부 시작시간 : ";
		ofs << f_hour << "시 " << f_min << "분 " << f_sec << "초" << endl;
		cout << "---오늘의 공부량---" << endl;

		ofs << "---event---" << endl;
		int len = c_timeline.size();
		for (int i = 0; i < len; ++i) {
			ofs << i + 1 << ".  " << c_timeline[i].first;
			if (c_timeline[i].second != 0) {
				ofs << "  소모시간 : " << c_timeline[i].second << "분";
			}
			ofs << endl;
		}
		ofs << "---plan---" << endl;
		len = c_studylist.size();
		for (int i = 0; i < len; ++i) {
			ofs << c_studylist[i].second.first << ". : " << c_studylist[i].first <<
				"   했나요? : " << (c_studylist[i].second.second ? "O" : "X") << endl;
		}

		int thour = temp_tm->tm_hour - f_hour;
		if (thour < 0) {
			thour += 24;
		}
		int tmin = temp_tm->tm_min - f_min;
		if (tmin < 0) {
			thour--;
			tmin += 60;
		}
		cout << "---total time---" << endl;
		cout << "총 공부시간 " << thour << "시간 " << tmin << "분" << endl;
		ofs << "총 공부시간 " << thour << "시간 " << tmin << "분" << endl;
		len = c_timeline.size();
		int sum = 0;
		for (int i = 0; i < len; ++i) {
			sum += c_timeline[i].second;
		}
		int h = 0;
		while (sum >= 60) {
			h++;
			sum -= 60;
		}

		int outh = thour - h;
		int outm = tmin - sum;
		if (outm < 0) {
			outm += 60;
			outh--;
		}

		cout << "실 공부시간 " << outh << "시간 " << outm << "분" << endl;
		cout << "헛 공부시간 " << h << "시간 " << sum << "분" << endl;
		ofs << "실 공부시간 " << outh << "시간 " << outm << "분" << endl;
		ofs << "헛 공부시간 " << h << "시간 " << sum << "분" << endl;
		return ofs;
	}

	void handsave(void) {
		cal_temp_time();
		ostringstream filename;
		filename << temp_tm->tm_year + 1900 << "_" << temp_tm->tm_mon + 1 << "_" << temp_tm->tm_mday << "_" << whatstudy << ".txt";

		ofstream ofs(filename.str());
		ofs << temp_tm->tm_year + 1900 << "년 " << temp_tm->tm_mon + 1 << "월 " << temp_tm->tm_mday << "일 ";
		ofs << '\"' << whatstudy << '\"' << "에 대한 공부" << endl;
		ofs << "공부 시작시간 : ";
		ofs << f_hour << "시 " << f_min << "분 " << f_sec << "초" << endl;
		ofs << "---event---" << endl;
		int len = c_timeline.size();
		for (int i = 0; i < len; ++i) {
			ofs << i + 1 << ".  " << c_timeline[i].first;
			if (c_timeline[i].second != 0) {
				ofs << "  소모시간 : " << c_timeline[i].second << "분";
			}
			ofs << endl;
		}
		ofs << "---plan---" << endl;
		len = c_studylist.size();
		for (int i = 0; i < len; ++i) {
			ofs << c_studylist[i].second.first << ". : " << c_studylist[i].first <<
				"   했나요? : " << (c_studylist[i].second.second ? "O" : "X") << endl;
		}

		int thour = temp_tm->tm_hour - f_hour;
		if (thour < 0) {
			thour += 24;
		}
		int tmin = temp_tm->tm_min - f_min;
		if (tmin < 0) {
			thour--;
			tmin += 60;
		}
		
		ofs << "총 공부시간 " << thour << "시간 " << tmin << "분" << endl;
		len = c_timeline.size();
		int sum = 0;
		for (int i = 0; i < len; ++i) {
			sum += c_timeline[i].second;
		}
		int h = 0;
		while (sum >= 60) {
			h++;
			sum -= 60;
		}

		int outh = thour - h;
		int outm = tmin - sum;
		if (outm < 0) {
			outm += 60;
			outh--;
		}
	
		ofs << "실 공부시간 " << outh << "시간 " << outm << "분" << endl;
		ofs << "헛 공부시간 " << h << "시간 " << sum << "분" << endl;
		ofs.close();
	}

	void make_report(void) {
		ostringstream filename;
		filename << temp_tm->tm_year + 1900 << "_" << temp_tm->tm_mon + 1 << "_" << temp_tm->tm_mday << "_" << whatstudy << ".txt";

		ofstream ofs(filename.str());
		study_feedback(ofs);
		ofs.close();
		cout << "--------------------------" << endl;
		cout << "수고하셨습니다!" << endl;
		system("pause");
	}
};

void version_info(void) {
	cout << "공부도우미 version 1.5 180531" << endl;
	cout << "마지막 시간 출력할 때 바로 꺼졌는데 멈췄다가 시작하게 하기(285의 system pause) " << endl;
	cout << "손으로 저장" << endl;
	cout << "1분마다 자동 저장" << endl;
	system("pause");
	system("cls");
}

bool end_sig = false;
void ast(psscc* p) { // autosavethread CBV로 하면 맨 처음 값을 가지고 while로 들어가서 값이 최신화되지 않았다 
	while (!end_sig) {
		(*p).handsave();
		int cnt = 0;
		while (!end_sig && cnt != 600) { // Sleep_time * cnt => wait time
			Sleep(100);
			cnt++;
		}
	}
}//sleep(10000) 하는 방법도 있지만 system이 끝날때 바로 thread 를 끝내지 않으면 asortcall error가 난다. 
//이렇게 하면 전역변수를 썼다는 단점은 있지만 그 에러를 방지하면서 0.1초 씩은 cpu가 쉬고 
//thread도 0.1초만에 끝낼 수 있다. 



void process() {
	version_info();
	cout << "무엇을 공부할 것인지 입력하세요 : ";
	string s;
	getline(cin, s);
	psscc p(s);
	thread t1(ast,&p);
	while (p.menu()) {}
	end_sig = true;
	system("cls");
	t1.join();
	p.make_report();
}

int main(void)
{
	system("mode con:cols=50 lines=10");
	system("title 공부도우미");
	process();
	
	return 0;
}
/*
ctime 에 대한 출처: http://dosnipe.tistory.com/39 [참고용 개발노트]
 


*/
/* 소모시간 입력받을때 사용하는 코드
cout << "소모시간을 분단위로 입력하세요 : ";
cin >> n;
while (0 > n || n > 300) {
cout << "잘못 입력하였습니다 다시입력하세요 : ";
clearInputBuffer();
cin.clear();
cin >> n;
}; */
//case문 안에서 초기화는 안된다. 하려면 {  } 중괄호로 씌운다음 지역변수로 이용해야.  case는 lable의 개념으로 jump하기 때문

// thread http://arer.tistory.com/45 블로그 참조

//autosave에서 시간지연 구현할 방법
//조건 1. 메인 프로세스에 영향 x
//sleep은 모든걸 정지시키는 것 같아 제외
//wait, delay 후보


//	TerminateThread(AutoSaveThread.native_handle, 0); handle 멤버함수
