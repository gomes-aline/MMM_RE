
EQUATION("Firm_Prior_Expected_Demand")
//EQUATION("Firm_Expected_Demand")
/*
Firm's expected sales are calculated from an average of effective sales from the two previous periods, applying a expected growth rate. This expected growth rate is obtained from comparing the average of the two previous periods with the average of the two before that, adjusted by an expectation parameter.
*/
	v[1]=VL("Firm_Effective_Orders", 1);                    //firm's effective orders lagged 1
	v[2]=VL("Firm_Effective_Orders", 2);                    //firm's effective orders lagged 2
	v[3]=V("sector_expectations");                          //firm expectations
	v[5]=(v[1]-v[2])/v[2];
	v[6]=max(min(v[5],1),-1);
	if(v[2]!=0)                                           	//if firm's effective orders lagged 2 is not zero
		v[4]=v[1]*(1+v[3]*v[6]);              //expected sales will be the effective orders in the last period multiplied by the growth rate between the two periods adjusted by the expectations parameter
	else                                                  	//if firm's effective orders lagged 2 is zero 
		v[4]=v[1];                                          //expected sales will be equal to effective orders of the last period
		
RESULT(max(0,v[4]))


EQUATION("Firm_Prior_Planned_Production")
//EQUATION("Firm_Planned_Production")
/*
It's calculated from expected sales for the period, considering the proportion of sales that is desired to be kept as inventories and discounting the acumulated stock of inventories in the last period. 
For the capital goods sector, program production is defined by effective orders.
Programed Production is subjected to a existing capactity restriction, but it is possible to increase production by incrising extra hours of labor, in any sector.
*/
	v[0]=V("id_capital_goods_sector");                    	//identifies the capital goods sector      
	v[1]=V("Firm_Prior_Expected_Demand");                   //calls the firm's expected sales
	v[2]=VL("Firm_Productive_Capacity", 1);                 //calls the firm's productive capacity of the last period
	v[5]=V("sector_desired_inventories_proportion");        //calls the firm's desired inventories ratio as a proportion of sales
	v[6]=VL("Firm_Stock_Inventories",1);                    //calls the firm's stock of inventories in the last period
	if(v[0]==0)                                           	//if it is not capital goods sector
		v[8]=v[1]*(1+v[5])-v[6];                            //planned production will be expected sales plus the desired proportion of investories minus the existing stock of inventories
	else                                                  	//if it is a capital goods sector
		{
		v[10]=V("sector_investment_frequency");
		v[7]=0;
		for(i=0;i<=v[10]-1;i++)
			{
			v[11]=VL("Firm_Effective_Orders_Capital_Goods",i);
			v[12]=v[11]/v[10];
			v[7]=v[7]+v[12];
			}
		v[8]=v[7]*(1+v[5])-v[6];
		//v[8]=v[1]*(1+v[5])-v[6];
		}
	v[9]=max(0, v[8]);                          			//planned production can never be more then the maximum productive capacity and can never be negative
RESULT(v[9])


EQUATION("Firm_Max_Production_Constrained_By_Capacity")
/* Firm's maximium production possible constrained by the installed productive capacity 
*/
	v[0]=VL("Firm_Productive_Capacity",1);                 //firm's productive capacity last period	
	if(v[0]>0)                                            //if firm's productive capacity is higher than zero
		v[1]=v[0];                                        //firm's maximium production constrained by installed productive capacity
	else                                                  //if firm's productive capacity is zero or negative 
		v[1]=0;                                           //firm's max production is zero    
RESULT(v[1])


EQUATION("Firm_Max_Production_Constrained_By_Inputs")
/* firm's maximium production possible constrained by the amount of inputs available
*/
	v[0]=VL("Firm_Avg_Input_Tech_Coefficient",1);         //firm's average input tech coefficient last period
	v[1]=VL("Firm_Stock_Inputs",1);                   	  //stock of remaining imputs from the last period	
	if(v[0]>0)                                            //if the input tech coefficient is higher than zero
		v[2]=v[1]/v[0];                                   //firm's maximium production constrained by available inputs 
	else                                                  //if the input tech coefficient is zero or negative 
		v[2]=1000000000;                                  //firm's production is not constrained by this input    
RESULT(v[2])


EQUATION("Firm_Max_Production_Constrained_By_Energy")
/* AJUSTAR COM REGRA DE RACIOAMENTO DE ENERGIA!!! 
firm's maximium production possible constrained by the amount of inputs available
*/
	v[0]=VL("Firm_Avg_Input_Tech_Coefficient",1);         //firm's average input tech coefficient last period
	v[1]=VL("Firm_Stock_Inputs",1);                   	  //stock of remaining imputs from the last period	
	if(v[0]>0)                                            //if the input tech coefficient is higher than zero
		v[2]=v[1]/v[0];                                   //firm's maximium production constrained by available inputs 
	else                                                  //if the input tech coefficient is zero or negative 
		v[2]=1000000000;                                  //firm's production is not constrained by this input    
RESULT(v[2])


EQUATION("Firm_Prior_Effective_Production")
/* Firm's (prior) effective production used to calculate the energy demand at current time.
*/
	v[0]=V("Firm_Prior_Planned_Production");
	v[1]=V("Firm_Max_Production_Constrained_By_Capacity");
	v[2]=V("Firm_Max_Production_Constrained_By_Inputs");
	v[3]=V("Firm_Max_Production_Constrained_By_Energy");
	v[4]=V("id_energy_sector");
	if(v[4]==1)                                           	//if it is the energy sector AJUSTAR!!!
		{
		v[5]= min(v[0],v[1]);                               //Firm's (prior) effective production is the lowest between planned production and the max productions constrained by capacity and inputs		
		v[6]= min(v[2],v[3]);
		v[7]= min(v[5],v[6]);
		//v[5]= min(v[0],v[1],v[2],v[3]);                   //Firm's (prior) effective production is the lowest between planned production and the max productions constrained by capacity and inputs		
		}
	else                                                   	//if it is not the energy sector
		{
		v[5]= min(v[0],v[1]);                               //Firm's (prior) effective production is the lowest between planned production and the max productions constrained by capacity and inputs		
		v[6]= min(v[2],v[3]);
		v[7]= min(v[5],v[6]);                               //Firm's (prior) effective production is the lowest between planned production and the max productions constrained by capacity and inputs	
		//v[5]= min(v[0],v[1],v[2],v[3]);                   //Firm's (prior) effective production is the lowest between planned production and the max productions constrained by capacity and inputs		
		}
RESULT(v[5])


EQUATION("Firm_Expected_Demand")
/*
Firm's expected sales are calculated from an average of effective sales from the two previous periods, applying a expected growth rate. This expected growth rate is obtained from comparing the average of the two previous periods with the average of the two before that, adjusted by an expectation parameter.
*/
	v[0]=V("Firm_Prior_Expected_Demand");
	//v[2]=v[0];
	v[1]=V("id_energy_sector");
	if(v[1]==1)                                           	//if it is the energy sector
		v[2]=v[0]+5;                                           
	else                                                  	//if it is not the energy sector
		v[2]=v[0];                                          //Firm's expected demand is equal to the prior
		
RESULT(v[2])


EQUATION("Firm_Planned_Production")
/*
*/
	v[0]=V("Firm_Prior_Planned_Production");
	//v[2]=v[0];
	v[1]=V("id_energy_sector");
	if(v[1]==1)                                           	//if it is the energy sector
		v[2]=v[0]+10;                                           
	else                                                  	//if it is not the energy sector
		v[2]=v[0];                                          //Firm's planned production is equal to the prior
	
RESULT(v[2])


EQUATION("Firm_Effective_Production")
/*
The actual production of each sector will be determined by the constraint imposed by the availability of inputs to the realization of the programmed production (or production of inputs desired, in the case of intermediate sectors). Such constraint is defined by the lower ratio between available inputs and the inputs required for production.
*/
	v[0]=V("Firm_Planned_Production");                                                              //firm's planned production
	v[1]=V("Firm_Available_Inputs_Ratio");
	v[2]=v[1]*v[0];                                                                            		//effective planned production, constrained by the ratio of available inputs
	
	v[10]=V("switch_capital_order");
	if(v[10]==0)	
	SORT("CAPITALS", "Capital_Good_Productivity", "DOWN");                                        	//rule for the use of capital goods, sorts firm's capital goods by productivity in a decreasing order
	if(v[10]==1)	
	SORT("CAPITALS", "capital_good_input_tech_coefficient", "UP"); 
	if(v[10]==2)	
	SORT("CAPITALS", "capital_good_energy_intensity", "UP"); 
	if(v[10]==3)	
	SORT("CAPITALS", "capital_good_carbon_intensity", "UP"); 
	v[3]=v[7]=0;                                                                                      	//initializes the CYCLE
	CYCLE(cur, "CAPITALS")                                                                        	//CYCLE trought the capital goods of the firm
	{
		v[4]=VS(cur, "capital_good_productive_capacity");                                          	//capital productivity capacity
		v[8]=VS(cur, "capital_good_carbon_intensity");
		v[5]=max(0,(min(v[4], v[2])));                                                             	//maximum capacity of each capital goods, constrained by effective planned production, and it can not be negative
		WRITES(cur, "Capital_Good_Production", v[5]);                                              	//the capacity of each capital goods is in fact its production
		v[2]=v[2]-v[5];                                                                            	//it subracts the production of the first capital good from the effective planned production before going to the next capital good
		v[3]=v[3]+v[5]; 																			//sums up the production of each capital good to determine firm's effective production
		v[7]=v[7]+v[5]*v[8];
	}
	WRITE("Firm_Emissions", v[7]);  
RESULT(v[3])

EQUATION_DUMMY("Firm_Emissions", "Firm_Effective_Production")
EQUATION_DUMMY("Capital_Good_Production", "Firm_Effective_Production")


EQUATION("Firm_Avg_Energy_Intensity")
/*
Firm's productivity will be an average of each capital good productivity weighted by their repective production	
*/
v[0]=v[1]=0;
CYCLE(cur, "CAPITALS")
{             
	v[2]=VS(cur,"capital_good_energy_intensity");
	if(v[2]!=0)
		{
		v[0]=v[0]+v[2];
		v[1]=v[1]+1;
		}
	else
		{
		v[0]=v[0];
		v[1]=v[1];
		}
}
v[3]=VL("Firm_Avg_Energy_Intensity",1);
v[4]=v[1]!=0? v[0]/v[1]: v[3];
RESULT(v[4])


EQUATION("Firm_Avg_Productivity")
/*
Firm's productivity will be an average of each capital good productivity weighted by their repective production	
*/
	v[0]=V("Firm_Effective_Production");                                		//firm's effective production
	v[1]=VL("Firm_Avg_Productivity", 1);                           				//firm's average productivity in the last period
	v[2]=0;                                                        				//initializes the CYCLE
	v[3]=0;                                                        				//initializes the CYCLE
	CYCLE(cur, "CAPITALS")                                          			//CYCLE trought firm's capital goods
	{
		v[4]=VS(cur, "Capital_Good_Productivity");                   			//capital good productivity
		v[5]=VS(cur, "Capital_Good_Production");                    			//capital good production
		v[2]=v[2]+v[4]*v[5];                                        			//sums up the product of each capital good productivity and production
		v[3]=v[3]+v[5];                                             			//sums up the production of each capital good
	}
	v[6]= v[3]!=0? v[2]/v[3] : v[1];                                            //firm's average productivity will be the average of each capital good productivity weighted by its respective production
RESULT(v[6])

EQUATION("Firm_Avg_Input_Tech_Coefficient")
/*
Firm's productivity will be an average of each capital good productivity weighted by their repective production	
*/        
v[0]=v[1]=0;
CYCLE(cur, "CAPITALS")
{             
	v[2]=VS(cur,"capital_good_input_tech_coefficient");
	if(v[2]!=0)
		{
		v[0]=v[0]+v[2];
		v[1]=v[1]+1;
		}
	else
		{
		v[0]=v[0];
		v[1]=v[1];
		}
}
v[3]=VL("Firm_Avg_Input_Tech_Coefficient",1);
v[4]=v[1]!=0? v[0]/v[1]: v[3];
RESULT(v[4])




EQUATION("Firm_Avg_Carbon_Intensity")
/*
Firm's productivity will be an average of each capital good productivity weighted by their repective production	
*/
v[0]=v[1]=0;
CYCLE(cur, "CAPITALS")
{             
	v[2]=VS(cur,"capital_good_carbon_intensity");
	if(v[2]!=0)
		{
		v[0]=v[0]+v[2];
		v[1]=v[1]+1;
		}
	else
		{
		v[0]=v[0];
		v[1]=v[1];
		}
}
v[3]=VL("Firm_Avg_Carbon_Intensity",1);
v[4]=v[1]!=0? v[0]/v[1]: v[3];
RESULT(v[4])


EQUATION("Firm_Capacity_Utilization")
/*
Firm effective production over firm productive capacity
*/
	v[0]=V("Firm_Effective_Production");
	v[1]=VL("Firm_Productive_Capacity",1);
	v[2]= v[1]!=0? v[0]/v[1] : 0;
RESULT(v[2])


