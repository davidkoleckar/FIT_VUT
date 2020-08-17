/*************************************************************
 * Projekt: IMS
 * Soubor: model_restaurace.cc
 * Cas vytvoreni: 1.12.2016 12:53
 * Cas posledni zmeny: 
 *
 * Autori: Janota Ondrej xjanot02@stud.fit.vutbr.cz
 *		   Koleckar David xkolec07@stud.fit.vutbr.cz	
 *
 * Popis: Hlavni program.
 *
 ***********************************************************/
/**
 * @file model_restaurace.cc
 *
 * @author Tym xkolec07
 *
 * @date 1.12.2016
 *
 * @brief Hlavni program.
 *
 */
#include <iostream>
#include <simlib.h>

class Group : public Process
{
	private:
		void Behavior();
		void GetPlace();
		void GetCostumers();
		void next_order();
		void Uklid();
		void Waitress_walk();
		void Call_waitress(int priority, Queue &q);
		void Let_waitress(Queue &q);

	public:
		int actual = 0;
		int remains = 0;
};

class Customer : public Process
{
	private:
		void Behavior();
		void objednani();
		void vyber();
		int menu();
		void pizza();
		void minutka();
		void hotovo();
		void consume();
		void Waitress_walk();
		void Call_waitress(int priority, Queue &q);
		void Let_waitress(Queue &q);

	public:
		Customer(Group *Skupinka);
		Group *Skupina;
};

const int COUNT_2 = 3;
const int COUNT_4 = 16;
const int COUNT_6 = 5;

Store Two[COUNT_2];
Store Four[COUNT_4];
Store Six[COUNT_6];

Facility Servirka[2];
Queue Order_waiting("Cekani na objednavku");
Queue Food_waiting("Cekani na jidlo");
Queue After_waiting("Cekani po jidle");
Queue Soup_waiting("Cekani na polevku");
Queue Uklid_waiting("Cekani na uklid");


Facility Kuchar("Kuchar");
Queue Menu_waiting("Priprava Menu");
Queue Minutka_waiting("Hotova minutka");
Queue cMinutka_waiting("Priprava minutky");
Queue Pizza_waiting("Peceni pizzy");

Store Pec("Pec", 4, Pizza_waiting);

Store Menu[3];

const int first_menu = 32;//Experiment.Fisrt_menu;
const int second_menu = 32;//Experiment.Second_menu;
const int third_menu = 16;//Experiment.Third_menu;

int count_of_menus = first_menu + second_menu + third_menu;

Queue Odchazi_hned("Odchazi_hned");
Histogram Prichozi("Prichozi", 0, 300, 50);

void Customer::Behavior()
{
	Prichozi(Time);

	Customer::objednani();
}

void Customer::objednani()
{
	/** objednavka zakaznika */
	Wait(Uniform(0.25*60, 0.35*60));

	/** zbyvajici objednavky ve skupine */
	Skupina->remains--;

	/** pokud jiz maji vsichni objednano */ 
	if (Skupina->remains == 0)
	{
		/** Servirka opusti skupinku */
		if (Servirka[0].in == (Event *)Skupina)
			Servirka[0].Release((Event *)Skupina);
		else
			if (Servirka[1].in == (Event *)Skupina)
				Servirka[1].Release((Event *)Skupina);

		/** aktualizace poctu lidi ve skupince pro dalsi potrebu */
		Skupina->remains = Skupina->actual;

		/** Pustime dalsi skupinku */
		if (!Order_waiting.Empty())
			(Order_waiting.GetFirst())->Activate();
	}

	/** Vyber jidla */
	Customer::vyber();
}

void Customer::vyber()
{
	double ran = Random();

	if (ran < 0.50)
	{
		/** pokud si neobjednali, objednaji si neco jineho*/
		/** pripadne se budou urazene ptat, jestli to menicko opravdu neni */
		if (menu())
			vyber();
	}
	else
		if (ran < 0.80)
			pizza();
		else
			minutka(); 
}

int Customer::menu()
{
	double ran = Random();
	bool neobjednano = true;

	/** pokud jsou jeste menicka */
	if (count_of_menus)
	{
		/** odectem ho */
		count_of_menus--;

		/*if (!Menu_waiting.Empty())
			(Menu_waiting.GetFirst())->Activate();*/

		/** 50% si da menu 1 */
		if (ran < 0.50)
		{
			if (Menu[0].Used() < first_menu)
			{
				Enter(Menu[0]);
				neobjednano = false;
			}
		}
		else
			/** 30% si da menu 2 */
			if (ran < 0.8)
			{
				if (Menu[1].Used() < second_menu)
				{
					Enter(Menu[1]);
					neobjednano = false;
				}
			}
			else
				/** 20% si da menu 3 */
				if (Menu[2].Used() < third_menu)
				{
					Enter(Menu[2]);
					neobjednano = false;
				}
	}

	/** pokud si neobjednali */
	if (neobjednano)
	{
		/** 70% lidi si objedna neco jineho */
		if (Random() < 0.7)
			return 1;
		else
			return 0;
	}
	else
	{
		/** Kuchar pripravi polevku */

		if (Kuchar.Busy())
		{
			Menu_waiting.Insert(this);
			this->Passivate();
		}

		/** chystani polevky */
		Kuchar.Seize(this);
			Wait(0.25*60);
		Kuchar.Release(this);

		if (!Menu_waiting.Empty())
			(Menu_waiting.GetFirst())->Activate();

		Call_waitress(2, Soup_waiting);

		/** donaska polevky */
		Waitress_walk();

		Let_waitress(Soup_waiting);

		/** priprava menu na talire */
		Wait(Uniform(4*60,7*60));
	}

	/** jidlo je pripraveno */
	Customer::hotovo();

	return 0;
}

void Customer::pizza()
{
	/** pizzar je jedinny */
	/** je omezen pouze kapacitou pece */

	/** priprava pizzy */
	Wait(Uniform(1,3));

	/** peceni pizzy */
	Enter(Pec);
		Wait(Normal(13*60,2*60));
	Leave(Pec);

	/** vyndani z pece, nakrajeni */
	Wait(Uniform(0.5*60,1*60));

	/** jidlo je hotove */ 
	Customer::hotovo();
}

void Customer::minutka()
{
	/** Pripravi si minutku */

	if (Kuchar.Busy())
	{
		cMinutka_waiting.Insert(this);
		this->Passivate();
	}

	Kuchar.Seize(this);
		Wait(Uniform(2,3));
	Kuchar.Release(this);

	if (!cMinutka_waiting.Empty())
		(cMinutka_waiting.GetFirst())->Activate();

	/** vareni */
	Wait(Uniform(25*60,35*60));

	/** minutku nachysta na talire */
	if (Kuchar.Busy())
	{
		Minutka_waiting.Insert(this);
		this->Passivate();
	}

	Kuchar.Seize(this, 1);
		Wait(Uniform(1*60,2*60));
	Kuchar.Release(this);

	if (!Minutka_waiting.Empty())
		(Minutka_waiting.GetFirst())->Activate();

	/** jidlo je hotove */
	Customer::hotovo();
}

void Customer::hotovo()
{
	/** pocka se na servirku */
	Call_waitress(0, Food_waiting);

	/** donaska jidla */
	Customer::Waitress_walk();

	Customer::Let_waitress(Food_waiting);

	/** konzumace */
	Customer::consume();
}

void Customer::consume()
{
	/** doba konzumace */
	Wait(Exponential(25*60));

	/** odecteme pocet jedliku */
	Skupina->remains--;

	/** kdyz ve skupine jiz nikdo neji */
	/** vratime zivot skupine */
	if (Skupina->remains == 0)
		Skupina->Activate();
}

void Customer::Waitress_walk()
{
	/** prumerna doba chuze servirky */
	Wait(Uniform(0.3*60, 0.5*60));
}

void Customer::Call_waitress(int priority, Queue &q)
{
	/** zabrani servirky */
	if (Servirka[0].Busy() && Servirka[1].Busy())
	{
		q.Insert(this);
		this->Passivate();
	}

	if (!Servirka[1].Busy())
		Servirka[1].Seize(this, priority);
	else
		if (!Servirka[0].Busy())
			Servirka[0].Seize(this, priority);
}

void Customer::Let_waitress(Queue &q)
{
	/** uvolneni servirky */
	if (Servirka[0].in == this)
		Servirka[0].Release(this);
	else
		if (Servirka[1].in == this)
			Servirka[1].Release(this);


	if (!q.Empty())
		(q.GetFirst())->Activate();
}

Customer::Customer(Group *Skupinka)
{
	/** ulozeni reference na skupinu zakazniku */
	Skupina = Skupinka;
	Activate();
}

void Group::Behavior()
{
	double ran = Random();

	/** procentualni rozlozeni poctu lidi ve skupine */
	if (ran < 0.08)
		actual = 1;
	else
		if (ran < 0.33)
			actual = 2;
		else
			if (ran < 0.41)
				actual = 3;
			else
				if (ran < 0.78)
					actual = 4;
				else
					if (ran < 0.89)
						actual = 5;
					else
						actual = 6;

	/** Skupina si zabere misto, pokud nejake najde */
	GetPlace();
}

void Group::GetPlace()
{
	bool two_people = false;
	bool four_people = false;
	bool six_people = false;

	int index = 0;

	switch(actual)
	{
		/** dojde 1 nebo 2 lide */
		case 1:
		case 2:
			/** je volny stul se 2 misty? */
			for (int i = 0; i < 3; i++)
				if (Two[i].Used() == 0)
				{
					Enter(Two[i], actual);
					two_people = true;
					index = i;
					break;
				}
			
			if (two_people)
				break;

			/** pokud neni volny stul se 2 misty */
			/** je volny stul se 4 misty? */
			for (int i = 0; i < COUNT_4; i++)
				if (Four[i].Used() == 0)
				{
					Enter(Four[i], actual);
					four_people = true;
					index = i;
					break;
				}
				else
					four_people = false;

			if (four_people)
				break;

			/** pokud neni volny stul se 4 misty */ 
			/** je volny stup se 6 misty? */
			for (int i = 0; i < COUNT_6; i++)
				if (Six[i].Used() <= 2) // <=2
				{
					Enter(Six[i], actual);
					six_people = true;
					index = i;
					break;
				}
				else
					six_people = false;
		break;

		case 3:
		case 4:
			/** je volny stul se 4 misty? */
			for (int i = 0; i < COUNT_4; i++)
				if (Four[i].Used() == 0)
				{
					Enter(Four[i], actual);
					four_people = true;
					index = i;
					break;
				}
				else
					four_people = false;

			if (four_people)
				break;

			/** pokud neni volny stul se 4 misty */ 
			/** je volny stup se 6 misty? */
			for (int i = 0; i < COUNT_6; i++)
				if (Six[i].Used() <= 1) //<= 1
				{
					Enter(Six[i], actual);
					six_people = true;
					index = i;
					break;
				}
				else
					six_people = false;
		break;

		case 5:
		case 6:
			/** je volny stup se 6 misty? */
			for (int i = 0; i < COUNT_6; i++)
				if (Six[i].Used() == 0)
				{
					Enter(Six[i], actual);
					six_people = true;
					index = i;
					break;
				}
				else
					six_people = false;
		break;
	}
	
	/** Pockame, nez si najde stul a pripadne i usadi */
	Wait(Exponential(2.5*60));

	if (two_people)
	{
		/** Ziskame pocet lidi ve skupine */
		Group::GetCostumers();

		/** Ze stolu c. index, */
		Leave(Two[index], actual);
	}
	else
		if (four_people)
		{
			Group::GetCostumers();

			Leave(Four[index], actual);
		}
		else
			if (six_people)
			{
				Group::GetCostumers();

				Leave(Six[index], actual);
			}
			else
				/** pro statistiku */
				Odchazi_hned.Insert(this);
}

void Group::GetCostumers()
{
	/** Nastavime frontu */
	Servirka[0].SetQueue(Order_waiting);
	Servirka[1].SetQueue(Order_waiting);

	Call_waitress(0, Order_waiting);

	Waitress_walk();

	remains = actual;

	/** Vytvareni jednotlivych zakazniku */
	for (int i = 0; i < actual; i++)
		new Customer(this);

	/** Skupina se rozdelila na zakazniky, pocka nez kazdy z nich doji. */
	this->Passivate();

	double ran = Random();

	Call_waitress(0, After_waiting);

	/** v 10% si objednaji jeste neco */
	if (ran < 0.10)
	{
		/** dalsi objednavka */
		next_order();

		Let_waitress(After_waiting);

		/** sedankuji dal */
		Wait(Uniform(15*60, 20*60));

		Call_waitress(0, After_waiting);

		Waitress_walk();
	}

	ran = Random();

	/** 70% plati hotove, zbytek kartou */
	if (ran < 0.70)
		Wait(Normal(15, 3));
	else
		Wait(Exponential(0.3*60));

	Let_waitress(After_waiting);

	/** po zaplaceni nejakou chvili sedi a pote odejdou */
	Wait(Uniform(0.5*60, 5*60));

	/** jde se utirat stul */
	Group::Uklid();
}

void Group::Call_waitress(int priority, Queue &q)
{
	if (Servirka[0].Busy() && Servirka[1].Busy())
	{
		q.Insert(this);
		this->Passivate();
	}

	if (!Servirka[1].Busy())
		Servirka[1].Seize(this, priority);
	else
		if (!Servirka[0].Busy())
			Servirka[0].Seize(this, priority);
}

void Group::Let_waitress(Queue &q)
{
	if (Servirka[0].in == this)
		Servirka[0].Release(this);
	else
		if (Servirka[1].in == this)
			Servirka[1].Release(this);


	if (!q.Empty())
		(q.GetFirst())->Activate();
}

void Group::Uklid()
{	
	Call_waitress(1, Uklid_waiting);

	Wait(Exponential(2*60));

	Let_waitress(Uklid_waiting);
	
	/** Lide odesli, stul je uklizen, zbyva uvolnit mista :) */
}

void Group::next_order()
{
	/** chuze */
	Waitress_walk();

	/** provedeni objednavky */
	Wait(Uniform(1*60,2*60)*actual);

	/** donaska jidla */
	Waitress_walk();
}

void Group::Waitress_walk()
{
	/** prumerna doba chuze */
	Wait(Exponential(0.5*60));
}

class Generator : public Event
{
	double cas;

 	void Behavior()
	{
		/** vytvoreni skupiny */
		(new Group)->Activate();

		/** prichod skupin zalezi na case */

		/** 10:30 - 11:00 */
		if (Time < 30*60)
			Activate(Time + Exponential(10.5*60));
		else
			/** 11:00 - 11:20 */
			if (Time < 50*60)
				Activate(Time + Exponential(8*60));
			else
				/** 11:20 - 12:20 */
				if (Time < 110*60)
					Activate(Time + Exponential(4*60));
				else
					/** 12:20 - 13:20 */
					if (Time < 170*60)
						Activate(Time + Exponential(5.5*60));
					else
						/** 13:20 - 13:40 */
						if (Time < 190*60)
							Activate(Time + Exponential(8*60));
						else
						{
							/** Aby se negenerovalo po 14:00 */
							double cas = Exponential(11*60);
							/** 13:40 - 14:00 */
							if (Time < (210*60 - cas))
								Activate(Time + cas);
						}
	}
};


void setDefaults()
{
	SetOutput("experiment.out");

	Servirka[0].SetName("Servirka 1");
	Servirka[1].SetName("Servirka 2");

	for (int i = 0; i < COUNT_2; i++)
	{
		Two[i].SetCapacity(2);
		Two[i].SetName("Stul pro 2");
	}

	for (int i = 0; i < COUNT_4; i++)
	{
		Four[i].SetCapacity(4);
		Four[i].SetName("Stul pro 4");
	}

	for (int i = 0; i < COUNT_6; i++)
	{
		Six[i].SetCapacity(6);
		Six[i].SetName("Stul pro 6");
	}

	Menu[0].SetName("Menu 1");
	Menu[0].SetCapacity(first_menu);
	Menu[1].SetName("Menu 2");
	Menu[1].SetCapacity(second_menu);
	Menu[2].SetName("Menu 3");
	Menu[2].SetCapacity(third_menu);
}

void outputs()
{
	Odchazi_hned.Output();

	Prichozi.Output();

	Servirka[0].Output();
	Servirka[1].Output();

	Kuchar.Output();
	
	Pec.Output();
	
	Menu[0].Output();
	Menu[1].Output();
	Menu[2].Output();

	for (int i = 0; i < COUNT_2; i++)
		Two[i].Output();

	for (int i = 0; i < COUNT_4; i++)
		Four[i].Output();

	for (int i = 0; i < COUNT_6; i++)
		Six[i].Output();

	Order_waiting.Output();
	Soup_waiting.Output();
	Menu_waiting.Output();
	Pizza_waiting.Output();
	cMinutka_waiting.Output();
	Minutka_waiting.Output();
	Food_waiting.Output();
	After_waiting.Output();
	Uklid_waiting.Output();

	SIMLIB_statistics.Output();

}

int main()
{
	setDefaults();

	Init(0, 250*60);

	RandomSeed(time(NULL));

	(new Generator)->Activate();

	Run();

	outputs();

	return 0;
}