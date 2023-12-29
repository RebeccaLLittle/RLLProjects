import java.net.Socket;
import java.net.UnknownHostException;
import java.io.IOException; 
import java.io.DataInputStream; 
import java.io.DataOutputStream; 

class play{
	
	//updates the current amount of money the user has 
	static int moneyUpdate(String a) {
		String[] placer = a.split(":");
		int money = Integer.parseInt(placer[1]);
		return money;
	}
	
	//counts the cards to determine when it is a good time to bet high
	static int[] cardCount(String b) throws IndexOutOfBoundsException{
		int cc[] = {1,1,1};
		String[] placer = b.split("all:");
		
		if(placer.length > 1) {
			String[] deck = placer[1].split(":");
			for(int i = 0; i < deck.length; i++) {
				if(deck[i].charAt(0) >= '2' && deck[i].charAt(0) <= '9') {
					cc[0] +=1;
				}
				if(deck[i].charAt(0) == 'J' || deck[i].charAt(0) == 'Q' || deck[i].charAt(0) == 'K') {
					cc[1] +=1;
				}
				if(deck[i].charAt(0) == '1' && deck[i].charAt(1) == '0') {
					cc[1] +=1;
				}
				if(deck[i].charAt(0) == 'A' ) {
					cc[2] +=1;	
				}
			}
		}
		return cc;
	}
	
	//Determines how much money to bet after the cards are counted
	static String moneyBet(int[] card, int m) {
		int money = m;
		String response = "";
		if((card[0] / (card[1] + card[2])) > 1 && card[0] > 200) {
			if (money < 3) {
				response = "bet:"+money;
			}
			else {
				response = "bet:3";
			}
		}
		else {
			response = "bet:1";
		}
		return response;
	}
	
	//Determines the values of player cards added together
	static int myCardTotal(String dealer) throws IndexOutOfBoundsException{
		int myTotal = 0;
		String[] placer = dealer.split("you:");
		String[] hand = placer[1].split(":");
	
		for(int i = 0; i < hand.length; i++) {
			
			if(i+1 < hand.length) {
				if (hand.length == 2) {
					if(hand[i].charAt(0) == hand[i+1].charAt(0)) {
						if(hand[i].charAt(0) == '1'|| hand[i].charAt(0) == 'J' || hand[i].charAt(0) == 'Q' || hand[i].charAt(0) == 'K') {
							myTotal = 20;
							return myTotal;
						}
						if(hand[i].charAt(0) == '5') {
							myTotal = 10;
							return myTotal;
						}
						else {
							myTotal = -1;
							return myTotal;
						}
					}
				
				}
			}	
			if(hand[i].charAt(0) >= '2' && hand[i].charAt(0) <= '9') {
				myTotal += Character.getNumericValue(hand[i].charAt(0));
			}
			else if(hand[i].charAt(0) == 'J' || hand[i].charAt(0) == 'Q' || hand[i].charAt(0) == 'K') {
				myTotal += 10;
			}
			else if(hand[i].charAt(0) == '1' && hand[i].charAt(1) == '0') {
				myTotal += 10;
			}
			else if(hand[i].charAt(0) == 'A' ) {
				if(myTotal > 10) {
					myTotal += 1;
				}
				else {
					myTotal += 11;
				}
			}
		
		}	
		return myTotal;
	}

	//determines the values of the dealers up card
	static int dealerCardTotal(String dealer) {
		int dealTotal = 0;
		String[] hand = dealer.split(":");
		
		if(hand[2].charAt(0) >= '2' && hand[2].charAt(0) <= '9') {
			dealTotal += Character.getNumericValue(hand[2].charAt(0));
		}
		else if(hand[2].charAt(0) == 'J' || hand[2].charAt(0) == 'Q' || hand[2].charAt(0) == 'K') {
			dealTotal += 10;
		}
		else if(hand[2].charAt(0) == '1' && hand[2].charAt(1) == '0') {
			dealTotal += 10;
		}
		else if(hand[2].charAt(0) == 'A' ) {
			dealTotal += 11;
		}
		
			
		return dealTotal;
	}
	
	//determines if player should stand, hit, double, or split
	static String moveLogic(int my, int deal, int mon, int len) {
		String response = "";
		if(my >= 17) {
			response = "stand";
		}
		else if((my >= 12 && my <= 16) && (deal >= 7)){
			response = "hit";
		}
		else if((my >= 12 && my <= 16)  && (deal >= 2 || deal <= 6)){
			response = "stand";
		}
		else if((my == 10 )){
			if(20 < mon) {
				if(len==2) {
					response = "double";
				}
				else {
					response = "hit";
				}
			}
			else {
				response = "hit";
			}
			
		}
		else if(my == 11 ){
			if(20 < mon) {
				if(len==2) {
					response = "double";
				}
			}
			else {
				response = "hit";
			}
		}
		else if (my == 9 && deal >=7) {
			response = "hit";
		}
		else if(my==9 && deal <7) {
			response = "stand";
		}
		else if((my == 5 )){
			if(20 < mon) {
				if(len == 2) {
					response = "double";
				}
			}
			else {
				response = "hit";
			}
		}
		else if(my >= 5 && my <=8) {
			response = "hit";
		}
		else if(my == -1) {
			if(mon > 1) {
				response = "split";
			}
			else {
				response = "hit";
			}
		}
		else {
			response = "stand";
		}
		return response;
	}
	
	//calculates the amount of cards dealt to player during a round so they will not double/split after hitting 
	static int amountOfCards(String dealer) {
		String[] placer = dealer.split("you:");
		String[] hand = placer[1].split(":");
		int length = hand.length;
		return length;
	}
	
	
}

public class Blackjack {
	
	private static void write(String s ) throws IOException { 
//		dos.writeUTF(s); 
//		dos.flush(); 
//		, DataOutputStream dos		
		
	} 
	
	private static String read() throws IOException { 
		//return dis.readUTF(); 
		//DataInputStream dis
		return "play:dealer:6D:you:4S:6D";
		
	} 

	public static void main(String[] args) throws NumberFormatException, UnknownHostException, IOException {
		
		//initalize variables 
		int money = 500;
		String dealer = "";
		int playerTotal = 0;
		int dealerTotal = 0;
		int len = 0;
		String response = "";
		
		/*
		 * String IpAddress = args[0]; String IpPort = args[1];
		 * 
		 * Socket socket = new Socket(IpAddress, Integer.valueOf(IpPort));
		 * DataInputStream dis = new DataInputStream(socket.getInputStream());
		 * DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
		 */
		
	   // adddoes back in for write is dis for read
		//runs untill given prompt "done"
		boolean run = true;
		while (run) {
			dealer = Blackjack.read();
			
			if (dealer.equals("login")) {
				response = "RebeccaLLittle:Rebecca";
				Blackjack.write(response);
			}
			if (dealer.charAt(0)=='b') {
				System.out.println(dealer);
				
				money = play.moneyUpdate(dealer);
				int deck[] = play.cardCount(dealer);
				response = play.moneyBet(deck, money);
				
				Blackjack.write(response);
			}
			if (dealer.charAt(0)=='p') {
				System.out.println(dealer);
				
				playerTotal = play.myCardTotal(dealer);
				dealerTotal = play.dealerCardTotal(dealer);
				len = play.amountOfCards(dealer);
				response = play.moveLogic(playerTotal, dealerTotal, money, len);
				
				Blackjack.write(response);
			}
			if(dealer.charAt(0)=='s') {
				System.out.println(dealer);
			}
			if(dealer.charAt(0)=='d') {
				run = false;
				System.out.println(dealer);
				break;
			}
			
		}
		
	 //socket.close();
	}
	
}
