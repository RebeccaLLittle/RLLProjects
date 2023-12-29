import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.Set;
import java.util.HashMap;
import java.util.HashSet;

public class IRoadTrip {
	

    public IRoadTrip (String [] args) {
    	if(args.length >=3) {
    		String borders = args[0];
        	String statename = args[1];
        	String capdist = args[2];
    	}
	
    }
   
    public List<String> findPath (Map<String, Map<String, Integer>> graph, String country, String country2) {
    	
    	//	WARNING -- works for countries without a special case ie chile to paragauy
       
    	//Shortest Distance a country is from origin 
        Map<String, Integer> shortestDistances = new HashMap<>();
        //Stores the previous"node" in the path
        Map<String, String> previousNodes = new HashMap<>();
        //Min heap that all of the countries are put into with Integer.Max_Value other than origin with one
        PriorityQueue<String> nodesToVisit = new PriorityQueue<>(Comparator.comparingInt(shortestDistances::get));
        //List of all of the countries that need to be visited for the shortest path
        List<String> path = new ArrayList<>();
        //once I reach the destination stop the algorithm
    	boolean reachedDes = false;
        
        //Add all countries to heap
        for(String c : graph.keySet()) {
        	if(c.equals(country)) {
        		shortestDistances.put(c, 0);
        	}else {
        		shortestDistances.put(c, Integer.MAX_VALUE);
        	}
        	previousNodes.put(c, null);
        	nodesToVisit.add(c);
        }
        
        //get every neighbor from current and update their distance 
        while(!nodesToVisit.isEmpty() && !reachedDes) {
        	String current = nodesToVisit.poll();
        	
        	for(Map.Entry<String,Integer> neighbor : graph.get(current).entrySet()) {
        		String curNei = neighbor.getKey();
        		int dis = neighbor.getValue();
        		int updated = shortestDistances.get(current) + dis;

        		if(updated < shortestDistances.get(curNei)) {
        			//add the updated distance to the specific country
        			shortestDistances.put(curNei, updated);
        			//record the path to the node
        			previousNodes.put(curNei, current);
        			if(curNei.equals(country2)) {
        				reachedDes = true;
        				break;
        			}
        			nodesToVisit.add(curNei);
        			
        		}
        	}
        }
        //find where to begin the path, build path, and reverse it because of backtacking
        for(String des : graph.keySet()) {
        	if(des.equals(country2)) {
        		if(shortestDistances.containsKey(country)) {
            		buildPath(previousNodes, des, shortestDistances, path, country);
            		Collections.reverse(path);
            		return path;
            	} else {
            		System.out.println("invalid country");
            	}
        	}
        	
        }
        return path;
 	
    	
    }
    
    public void buildPath(Map<String, String> previousNodes, String des, Map<String, Integer> shortestDistances, List<String> path, String country) {
    	String step = "";
    	//find path until we have backtracked to origin 
    	while(!des.equals(country)) {
    		String temp = des;
    		//get previous "node"
    		des = previousNodes.get(des);
    		step = String.format("* %s --> %s  (%d km.)", des, temp,shortestDistances.get(temp));
    		path.add(step);
    	}
    }


    public void acceptUserInput() {

    	Scanner scan = new Scanner(System.in);
    	String input = "";
    	String input2 = "";
    	System.out.println("Creating map");
    	Map<String, Map<String, Integer>> map = new HashMap<>();
    	getBorders(map);
    	System.out.println("Done creating map");
    	
    	//run program until user enters exit
    	while(!input.equals("EXIT")) {
    		System.out.print("Enter the name of the first country (type EXIT to quit):");
    		input = scan.nextLine();
    		if(input.equals("EXIT")) {
    			System.exit(0);
    		}
    		//loops until user enters a valid input
    		while(badCountry(input) == -1) {
    			System.out.println("Invalid country name. Please enter a valid country name.");
    			System.out.print("Enter the name of the first country (type EXIT to quit):");
        		input = scan.nextLine();
        		if(input.equals("EXIT")) {
        			System.exit(0);
        		}
    		}
    		System.out.println("Enter the name of the second country (type EXIT to quit):");
    		input2 = scan.nextLine();
    		while(badCountry(input2) == -1) {
    			System.out.println("Invalid country name. Please enter a valid country name.");
    			System.out.print("Enter the name of the second country (type EXIT to quit):");
        		input2 = scan.nextLine();
        		if(input2.equals("EXIT")) {
        			System.exit(0);
        		}
    		}
    		List<String> path = findPath(map, input, input2);
    		for(String entry: path) {
    			System.out.println(entry);
    		}
    		
    	}
    }
    
 
    public int badCountry(String input) {
    	//check that countries entered exist
    	
		try {
			File borders = new File("borders.txt");
			File names = new File("state_name.tsv");
			Scanner scan = new Scanner(borders);
			Scanner scan2 = new Scanner(names);
			
			//checking input is not a special case in files 
			String result = specialCase(input);
			if(result.equals("error")) {
				return -1;
			}
			//checking file 1 for valid input 
			while(scan.hasNextLine()) {
				String line = scan.nextLine();
				String file1[] = line.split("=");
				String country = file1[0].trim();
				if(input.equals(country)) {
					return 1;
				}
				
			}
			//checking file 2 for valid input 
			while(scan.hasNextLine()) {
				String line2 = scan2.nextLine();
				String[] info = line2.split("\\t");
				if(info[2].equals(input)) {
					return 1;
				}
				
			}
			scan.close();
			scan2.close();
			
			//if not returned by the point return -1 to indicate nonvalid
			return -1;
			
		} catch (FileNotFoundException e) {
			System.out.println("error with files " + e);
			e.printStackTrace();
		}
		return -1;
		
		
		
    	
    }
    public void notNeighbors() {
    	//check for valid path
    }
    
    
    
    public static void getBorders(Map<String, Map<String, Integer>> map ) {
    	try {
    		File borders = new File("borders.txt");
			Scanner scan = new Scanner(borders);
			String[] doNotExist = {"Eswatini", "Gibraltar", "Holy See (Vatican City)", "Kyrgyzstan", "Liechtenstein", "Romania", "San Marino", "Timor Leste", "West Bank", "Gaza Strip"};
			
			
			while(scan.hasNextLine()) {
				
				//initalize variables and make array of countries on the line in borders
				Map<String, Integer> inner = new HashMap<>();	
				String result = "error";
				String line = scan.nextLine();
				String[] neighbors = line.split("[=;]");
				
				//checking if current line is a island
				while(neighbors.length ==2 && neighbors[1].equals(" ") ) {
					line = scan.nextLine();
					neighbors = line.split("[=;]");
				
				}
				
				
				//checking if origin is a special case (name mismatch or country that does not exist in both files)
				boolean isBadO = false;	

		    	for(int j =0; j < doNotExist.length; j++) {
		    		if(neighbors[0].equals(doNotExist[j])) {
		    			isBadO = true; 
		    		}

		    	}

				result =specialCase(neighbors[0]).trim();
				 if(!result.equals("pass")) {
					neighbors[0] = result.trim();
				} else {
					neighbors[0] = neighbors[0].trim();
				}
				
		
				if(!isBadO) {
					map.put(neighbors[0], inner);
				}
				
				
				//iterate through all of origins neighbors 
				for(int i = 1; i < neighbors.length && !isBadO; i++) {	
					
					//make sure none of the neighbors are invalid countries 
					boolean isBad = false;
			    	for(int j =0; j < doNotExist.length; j++) {
			    		if(neighbors[i].equals(doNotExist[j])) {
			    			isBad = true; 
			    		}

			    	}
			    	//if it is not invalid go through steps to get rid of formatting other than country name 
			    	if(!isBad) {
						for(int j = 0; j < neighbors[i].length(); j++) {
							if(Character.isDigit(neighbors[i].charAt(j))) {
								neighbors[i] = neighbors[i].substring(0, j);
								neighbors[i] = neighbors[i].trim();

							}
						
						}
						//check if neighbor is a special case and needs to be changed to alias
						result =specialCase(neighbors[i]).trim();
						if(!result.equals("pass")) {
							neighbors[i] = result.trim();
						} else {
							neighbors[i] = neighbors[i].trim();
							
						}
					
						//get the distance from by connecting to other files
						int dist = getCountryCode(neighbors, i);
						inner.put(neighbors[i], dist);
			    	}
					
	
					
				}

			}
			scan.close();

			
		} catch (FileNotFoundException e) {
			System.err.println("Error with files" + e);
		}
		
    }
    
    public static int getCountryCode(String[] neighbors, int i) {
    	
		try {
			File names = new File("state_name.tsv");
	    	String origin = neighbors[0];
			String neighbor =neighbors[i];
			Scanner scan2;
			scan2 = new Scanner(names);
			//origin ID
			int oID = -1;
			//neighbor ID
			int nID = -1;
			
			//Find the IDs of origin and neighbor by checking the lines of state_name
			while(scan2.hasNextLine()) {				
				String line2 = scan2.nextLine();
				String[] info = line2.split("\\t");
				if(info[2].equals(origin)) {
					oID = Integer.parseInt(info[0]);
				}
				if(info[2].equals(neighbor)){
					nID = Integer.parseInt(info[0]);
				}
			}
			//if both IDs where find get the distance between them
			if(oID != -1 && nID != -1) {
				int dist = getDist(oID, nID);
				scan2.close();
				return dist;
			}
			scan2.close();
			
			
		} catch (FileNotFoundException e) {
			System.out.println("error with files" + e);
			e.printStackTrace();
		}
		return -1;
		
    }
    public static int getDist(int oID, int nID) {
    	
		try {
			File caps = new File("capdist.csv");
	    	Scanner scan3;
			scan3 = new Scanner(caps);
			String line3 = scan3.nextLine();
			
			//check if line in capdist contains distance between two neighboring countries specified
			while(scan3.hasNextLine()) {
				line3 = scan3.nextLine();
				String[] dists = line3.split(",");
				if(oID == Integer.parseInt(dists[0]) && nID == Integer.parseInt(dists[2])) {
					int dist = Integer.parseInt(dists[4]);
					scan3.close();
					return dist;
				}
			}
			
			
		} catch (FileNotFoundException e) {
			System.out.println("error with files" + e);
			e.printStackTrace();
		}
	
		return -1;
		
    }
    
    
    public static String specialCase(String country) {
    	
    	//Switches to one alias 
    	 String[][] misMatch = {{"UK","United Kingdom"},{"Democratic Republic of Congo", "Congo, Democratic Republic of (Zaire)", "Congo, Democratic Republic of the"}, {"Bosnia and Herzegovina","Bosnia-Herzegovina"}, {"Burkina Faso", "Burkina Faso (Upper Volta)"},{"Germany", "German Federal Republic"},{"Guinea", "Guinea-Bissau"},{"Iran", "Iran (Persia)"},{"Italy", "Italy Sardinia"},{"North Korea", "Korea, People's republic of"},{"North Macedonia", "Macedonia (Former Yugoslav Republic of)"},{"Republic of Congo", "Congo"},{"South Korea", "Korea, Republic of"},{"Tanzania", "Tanzania/Tanganvika"},{"UAE", "United Arab Emirates"}, {"United States","United States of America", "US"}, {"Cambodia","Cambodia (Kampuchea)"}, {"Cote d'Ivoire","Cote D'Ivoire"},{"Germany", "German Federal Republic"},{"Guinea", "Guinea-Bissau"},{"Iran", "Iran (Persia)"},{"Italy", "Italy Sardinia"},{"North Korea", "Korea, People's republic of"},{"Republic of Congo", "Congo"},{"South Korea", "Korea, Republic of"},{"Tanzania", "Tanzania/Tanganyika"},{"UAE", "United Arab Emirates"}, {"Turkey","Turkey (Ottoman Empire)"}, {"Yemen", "Yemen (Arab Republic of Yemen)"}};
    	 
    	 //checks for any possible combination of the names, then switches to the alais, always the second element in array
    	 for(int i = 0; i < misMatch.length; i++) {
    		 for(int j = 0; j < misMatch[i].length; j++) {
    			 if(country.equals(misMatch[i][j])) {
        			 country = misMatch[i][1];
        			 return country;
        		 } 
    		 }
    		 
    	 }
    	 return "pass";
    }

    public static void main(String[] args) {
    	IRoadTrip a3 = new IRoadTrip(args);
        a3.acceptUserInput();
    	
    }

}

