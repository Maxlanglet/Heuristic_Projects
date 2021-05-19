/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
//#include <set>
#include "pfspinstance.h"
#include "Timer.hpp"

using namespace std;


int time_limit = 2; // for instances of 50 jobs, ATTENTION: change to 333 for instances of 100 jobs


int generateRndPosition(int min, int max)
{
	//srand(1);
	return ( rand() % max + min );
}


/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void randomPermutation(int nbJobs, vector< int > & sol)
{
	vector<bool> alreadyTaken(nbJobs+1, false); // nbJobs elements with value false
	vector<int > choosenNumber(nbJobs+1, 0);

	int nbj;
	int rnd, i, j, nbFalse;

	nbj = 0;
	for (i = nbJobs; i >= 1; --i)
	{
		rnd = generateRndPosition(1, i);
		nbFalse = 0;

		/* find the rndth cell with value = false : */
		for (j = 1; nbFalse < rnd; ++j)
			if ( ! alreadyTaken[j] )
				++nbFalse;
		--j;

		sol[j] = i;

		++nbj;
		choosenNumber[nbj] = j;

		alreadyTaken[j] = true;
	}
}

void simpleRZ(PfspInstance &instance, vector< int > & sol){	//Function used to generate the simplified RZ initial solution
															//since this method concerns the preceding implementation exercise, it is not annotated

	std::vector< std::vector <long int> > matrix = instance.getMatrix();
	std::vector< long int > priority = instance.getPriority();
	std::vector< float> ti;
	std::vector< int> index;
	int nbJob = instance.getNbJob();
	int nbMac = instance.getNbMac();

	ti.resize(nbJob+1);
	ti[0]=0;
	index.resize(nbJob+1);
	index[0]=0;

	for (int j = 1; j <= nbJob; ++j)
		{
			float sum = 0;
			for (int m = 1; m <= nbMac; ++m)
			{
				sum+=matrix[j][m];
			}
			ti[j]=sum/priority[j];
			index[j]=j;
		}

		int i =1;
		sol[0]=0;
		while (ti.size() != 1){
			std::vector<float>::iterator result = std::min_element(ti.begin()+1, ti.end());
			int temp=std::distance(ti.begin(), result);
			sol[i]=index[temp];
			i++;
			ti.erase(ti.begin()+temp);
			index.erase(index.begin()+temp);
		}

		std::vector< int> jobs;
		jobs.resize(nbJob+1);
		jobs[1]=sol[1];
		int size=2;

		for (int i=2; i<=nbJob;i++){
			int temp = sol[i];
			int wct=0;
			size++;
			for (int j=1; j<size;j++){
				jobs.insert(jobs.begin()+j, temp);
				jobs.erase(jobs.end()-1);
				if (j==1){
					wct = instance.computeWCT(jobs);
					index = jobs;
				}
				if (wct > instance.computeWCT(jobs)){
					index = jobs;
					wct = instance.computeWCT(jobs);
				}
				jobs.erase(jobs.begin()+j);
				jobs.insert(jobs.end(), 0);
			}
			jobs = index;
		}
		sol = index;

}


vector< int > Transp(PfspInstance instance, vector< int > & sol){	//transpose neighborhood, used for VND
																	//but not detailed since preceding implementation exercise
	int current=instance.computeWCT(sol);
	int best = current;
	vector< int > copy;
	vector< int > best_sol = sol;
	//Transpose
	for (int i=1;i<sol.size()-1;i++){
			copy = sol;
			iter_swap(copy.begin() + i, copy.begin() + i+1);
			current = instance.computeWCT(copy);

			if (current < best){
				best_sol = copy;
				best = current;
				break;
			}
		}
	return best_sol;
}

vector< int > Exch(PfspInstance instance, vector< int > & sol){	//exchange neighborhood, used for VND
																//but not detailed since preceding implementation exercise
	int current=instance.computeWCT(sol);
	int best = current;
	vector< int > copy;
	vector< int > best_sol = sol;
	//Exchange
	for (int j=1; j<sol.size();j++){
			for (int i=1;i<sol.size();i++){
				copy = sol;
				if (i!=j){
					iter_swap(copy.begin() + j, copy.begin()+i);
					current = instance.computeWCT(copy);

					if (current < best){
						best_sol = copy;
						best = current;
						break;
					}
				}
			}
		}
	return best_sol;
}

vector< int > Ins(PfspInstance &instance, vector< int > & sol){	//Insert neighborhood, used for VND and Local search method
																
	int current=instance.computeWCT(sol);
	int best = current;
	vector< int > copy;
	vector< int > best_sol = sol;
	//Insert
	for (int j=1; j<sol.size();j++){		//iterates over the solution and insert element j at position i
			for (int i=1;i<sol.size();i++){
				copy = sol;
				if (i!=j){
					if (i<j){
	        			std::rotate(copy.rend() - j - 1, copy.rend() - j, copy.rend() - i);
					}
					else{
						std::rotate(copy.begin() + j, copy.begin() + j+1, copy.begin()+i+1);//+i+1 for comprehension, since start is at 1 by convention
					}

					current = instance.computeWCT(copy);
					if (current < best){		//computes the wct and replaces preceding best solution if wct is better
						best_sol = copy;
						best = current;
						break;
						
					}
				}
			}
		}
	return best_sol;
}

vector<int> Localsearch(PfspInstance &instance, vector<int> &sol){
	sol = Ins(instance, sol);		//Use of insert method because it's the best one comparing to exchange and transpose
	return sol;
}

vector<int> Destruction_Creation(PfspInstance &instance, vector<int> &sol, int d){


	int nbJob = instance.getNbJob();
	vector<int> pi_r;
	vector<int> prime = sol;
	vector <int> best_sol;
	pi_r.resize(d);
	pi_r[0]=0;

	for (int i=0;i<d;i++){		//this loop will generate d random positions which will select our d jobs of destruction phase
		int rnd = generateRndPosition(1, prime.size()-1);
		pi_r[i]=prime[rnd];
		prime.erase(prime.begin()+rnd);	//removing the job selected, destruction phase
	}

	best_sol = prime;
	int current=instance.computeWCT(prime);
	int best=current;

	for (int i=0; i<pi_r.size();i++){
		//now insert job pi_r at all possible positions and keep best candidate solution after insertion
		int temp = pi_r[i];
		prime = best_sol;
		best_sol.resize(prime.size()+1);

		for (int j=1;j<prime.size()+1;j++){
			prime.insert(prime.begin()+j, temp);
			current = instance.computeWCT(prime);
			if (j==1){	//special case treated since for first insertion, wct always higher than preceding one
				best = current;
			}

			if (current<=best){
				best_sol = prime;
				best = current;
			}
			prime.erase(prime.begin()+j);
		}

	}
	return best_sol;
}

void IGA(PfspInstance &instance, vector< int > & sol){	//Iterated greedy algorithm


	vector<int> star = sol;
	vector<int> star_star = sol;
	vector<int> best_sol = sol;

	int current=instance.computeWCT(sol);
	int best = current;

	sol = Localsearch(instance, sol);

	int real=0;

	while(real/1000000<time_limit){

		Timer timer;
		sol = best_sol;

		vector<int> temp = sol;
		star=sol;

		//since their is random involved, loop to make multiple choices and take the best one
		for (int l=0; l<10;l++){
			temp = Destruction_Creation(instance, sol, 8);
			if (instance.computeWCT(temp) <= instance.computeWCT(star)){
				star = temp;
			}
		}

		star_star = Localsearch(instance, star);

		//Accept Criterion
		current = instance.computeWCT(star_star);
		if (current < best){
			best_sol = star_star;
			best = current;
		}
		timer.Stop();
		real+=timer.getDuration();
	}


	cout << "Time :"<<real << endl;	//verifying the time taken is right

	sol = best_sol;
}



vector<vector <int> > tabu_list;	//initialize the tabu list


vector<int> NSP(PfspInstance &instance, vector<int> &sol, int alpha){	// neighborhood search procedure



	int current=instance.computeWCT(sol);
	int best = current;
	vector< int > copy;
	vector< int > best_sol = sol;

	for (int j=1; j<sol.size();j++){				//this double loop will insert element j at position i-alpha to i+alpha and keep best insertion
			for (int i=j-alpha;i<j+alpha;i++){
				while(i<=0){//To ensure that i is not inferior to 0
					i++;
				}
				if (i>=sol.size()){//stop if we are bigger than the size of the vector
					break;
				}
				copy = sol;
				if (i!=j){
					if (i<j){
	        			std::rotate(copy.rend() - j - 1, copy.rend() - j, copy.rend() - i);
					}
					else{
						std::rotate(copy.begin() + j, copy.begin() + j+1, copy.begin()+i+1);//+i+1 for comprehension, since start is at 1 by convention
					}

					current = instance.computeWCT(copy);
					if (current < best ){
						if(find(tabu_list.begin(), tabu_list.end(), copy)!=tabu_list.end() && tabu_list.size()>1){
							tabu_list.push_back(copy);
							sol=copy;
							best_sol = copy;
							best = current;
						}
						else if (tabu_list.size()<=1){
							tabu_list.push_back(copy);
							sol=copy;
							best_sol = copy;
							best = current;
						}
						
					}
				}
			}
		}
	return best_sol;
}

void Tabu_Search(PfspInstance &instance, vector<int> &sol){		//Tabu search procedure
	int c = instance.computeWCT(sol);

	int stuck = 0;
	int maxStuck=10;

	int current;

	vector<int> best_move;
	vector<int> temp;

	int real=0;	//time elapsed

	while(real/1000000<time_limit){	//stopping criterion

		Timer timer;
		best_move = NSP(instance, sol, 3);	//computed nsp, note tabulist is updated in this function

		current=instance.computeWCT(best_move);
		if(current<c){
			stuck=0;
			c=current;
			sol=best_move;
		}
		else{
			stuck++;
			if (stuck>=maxStuck){
				for (int l=0; l<10;l++){
					temp = Destruction_Creation(instance, sol, 2);
					if (instance.computeWCT(temp) <= c){
						sol = temp;
					}
				}
				stuck=0;
			}
		}
		timer.Stop();
		real+=timer.getDuration();
	}
}

void VND(PfspInstance instance, vector< int > & sol){	//VND with stopping criterion updated for small comparison

	vector< int > copy;
	int best = instance.computeWCT(sol);


	int real=0;

	while(real/1000000<time_limit){

		Timer timer;
		start:
		copy = Transp(instance, sol);
		int current = instance.computeWCT(copy);
		if (current < best){
			//cout << "transpose" << endl;
			sol=copy;
			best=current;
			goto start;
		}
		copy = Exch(instance, sol);
		current = instance.computeWCT(copy);
		if (current < best){
			//cout << "insert" << endl;
			sol=copy;
			best=current;
			goto start;
		}
		copy = Ins(instance, sol);
		current = instance.computeWCT(copy);
		if (current < best){
			//cout << "exch" << endl;
			sol=copy;
			best=current;
			goto start;
		}
		//cout << best << end;
		timer.Stop();
		real+=timer.getDuration();
	}
}



/***********************************************************************/

int main(int argc, char *argv[])
{
	int i;
	long int WeightedSumCompletionTimes;
	bool first;

	//cout << "\n\n" << endl;


	if (argc <= 3)	//Usage, error in case of non respect
	{
		cout << "Usage: ./flowshopWCT <instance_file> <IGA/Tabu> <random/srz>" << endl;
		return 0;
	}

	string sec=argv[2];
	string third = argv[3];

	/* initialize random seed: */
	srand ( time(NULL) );

	/* Create instance object */
	PfspInstance instance;

	/* Read data from file */
	if (! instance.readDataFromFile(argv[1]) )
		return 1;
	argv[1]+=13;
	cout << argv[1] << " , ";
	/* Create a vector of int to represent the solution
		WARNING: By convention, we store the jobs starting from index 1,
						 thus the size nbJob + 1. */
	vector< int > solution ( instance.getNbJob()+ 1 );

	/* Fill the vector with a random permutation */

	if (third=="random"){
		randomPermutation(instance.getNbJob(), solution);

	}
	else if (third=="srz"){		//Only srz tested in our case
		simpleRZ(instance, solution);

	}
	else{
		cout << "Usage: ./flowshopWCT <instance_file> <IGA/Tabu> <random/srz>" << endl;
		return 0;
	}


	vector<int> temp;

	if (sec=="IGA"){
		//temp=solution;	//for testing with VND
		IGA(instance, solution);
		//VND(instance, solution); //for testing with VND

	}
	else if (sec=="Tabu"){
		Tabu_Search(instance, solution);
	}
	else{
		cout << "Usage: ./flowshopWCT <instance_file> <IGA/Tabu> <random/srz>" << endl;
		return 0;
	}

	/* Compute the WCT of this solution */

	WeightedSumCompletionTimes = instance.computeWCT(solution);
	cout << WeightedSumCompletionTimes << endl;
	//cout << instance.computeWCT(temp) << endl;	//for VND testing
	return 0;
}
