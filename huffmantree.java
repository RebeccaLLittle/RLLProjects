import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class huffNode{
	
	
	//class node that has right node, left node, and value
	private class node{
		String chr;
		node left;
		node right;
		
		//constructor that sets value when given one
		node(String value){
			chr = value;
			left = null;
			right = null;
		}
	}
	
	//instance variables 
	node root = new node(null);
	node pointer = root;
	
	//get input on one line
	String getInput(Scanner scan) {
		String inp = "";
		
		while(scan.hasNextLine()) {
			inp += scan.nextLine();

		}
		return inp+"0";
	}
	

	//builds huffman code tree by placing 0s on left if null and 1s on right if null
	public void treeBuilder(Scanner scan) {
		String placer = "";
 
        while (scan.hasNextLine()) {
            placer = scan.nextLine();
            String[] arr = placer.split(":");
            String code = arr[1];
            pointer = root;
            for(int i = 0; i < arr[1].length(); i++) {
	    		if (code.charAt(i) == '0') {
	    			if(pointer.left == null) {
	    				node newNode = new node("0");
	    				pointer.left = newNode;
	    				pointer = pointer.left;
	    			}
	    			else {
	    				pointer = pointer.left;
	    			}
	    		}
	    		if (code.charAt(i) == '1') {
	    			if(pointer.right == null) {
	    				node newNode = new node("1"); 
	    				pointer.right = newNode;
	    				pointer = pointer.right;
	    			}
	    			else {
	    				pointer = pointer.right;
	    			}
	    		}
            }
            char val = (char) Integer.parseInt(arr[0]);
            String val2 = Character.toString(val);
            pointer.chr = val2;
        }
		
		
	}
	
	//decodes the 0s and 1s, going down the tree untill it hits a leaf (end)
	String treeDecode(String line){
		String dec = "";
		pointer = root;
		for(int i = 0; i < line.length(); i++) {
			if(pointer.left == null && pointer.right == null) {
				dec += pointer.chr;
				pointer = root;
			}
			if(line.charAt(i) == '0'){
				pointer = pointer.left;
			}
			else if(line.charAt(i) == '1') {
				pointer = pointer.right;
			}
			
		}
		
		return dec;
	}
	

}

public class Decode {
	public static void main(String[] args) {
		try {
			
			//create the files
			File cB = new File("codebook.txt");
			File input = new File(args[0]);
			FileWriter writer = new FileWriter(args[1]);
			
			//create the scanners
			Scanner scan1 = new Scanner(cB);
			Scanner scan2 = new Scanner(input);
			
			huffNode huff = new huffNode();
			
			//call the methods
			huff.treeBuilder(scan1);
			String encode = huff.getInput(scan2);
			String decoded = huff.treeDecode(encode);
			writer.write(decoded);
			
			//close the scanners
			scan1.close();
			scan2.close();
			writer.close();
			
		}
		catch (FileNotFoundException e) {
			System.err.println("File not found " + e);
		}
		catch(IOException ioe) {
			System.err.println("Error with files" + ioe);
		}
		catch(ArrayIndexOutOfBoundsException aiobe) {
			System.err.println("Not enough args " + aiobe);
		}
		
	}
}

