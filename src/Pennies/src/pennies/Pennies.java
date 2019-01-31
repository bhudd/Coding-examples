package pennies;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

/**
 * Plays out a scenario of pennies, where 1 player picks a sequence of 3 coin tosses (Ex: Heads, Tails, Heads) and
 * another user picks a second sequence of coin tosses. A coin is then tossed until
 * either of the players sequences shows up. The first sequence to be shown wins.
 * @author G751JT
 *
 */
public class Pennies {
	
	public static class Coins
	{
		Boolean[] coins = new Boolean[3];
		Coins(String coinChoice)
		{
			coins[0] = coinChoice.charAt(0) == 'H';
			coins[1] = coinChoice.charAt(1) == 'H';
			coins[2] = coinChoice.charAt(2) == 'H';
		}
		
		@Override
		public String toString()
		{
			String str = coins[0] ? "H" : "T";
			str += coins[1] ? "H" : "T";
			str += coins[2] ? "H" : "T";
			return str;
		}
		
		public String sideUp(int index)
		{
			return coins[index] ? "H" : "T";
		}
		
		public String sideDown(int index)
		{
			return coins[index] ? "T" : "H";
		}
		
		public boolean compare(Boolean[] coins)
		{
			return Arrays.equals(this.coins, coins);
		}
	}
	
	private static int RUN_COUNT = 100000;
	
	public static void main(String[] args)
	{
		new Pennies();
	}
	
	Pennies()
	{
		double player1Wins = 0;
		double player2Wins = 0;
		
		Coins player1 = new Coins("TTH");
		Coins player2 = getPlayer2Choice(player1);
		
		long startTime = System.nanoTime();
		
		for(int i=0; i < RUN_COUNT; i++)
		{
			if(beginGame(player1, player2))
			{
				player1Wins++;
			}
			else
			{
				player2Wins++;
			}
		}

		System.out.println("total time: " + (System.nanoTime() - startTime)/1000000);
		System.out.println("player 2 win percentage: " + player2Wins / RUN_COUNT);
	}
	
	/**
	 * Runs a simulation of flipping coins sequentually. Once 3 consecutive flips equals
	 * either player 1 or player 2's sequence, it will return
	 * @return true if player 1 won, false if player 2 won.
	 */
	private boolean beginGame(Coins p1, Coins p2)
	{
		Random random = new Random();
		ArrayList<Boolean> flipBuffer = new ArrayList<>();
		// fill the buffer
		while(flipBuffer.size() < 3)
		{
			flipBuffer.add(random.nextBoolean());
		}
		
		int bufIndex = 0;
		
		while(true)
		{
			Boolean[] comparable = getBufferWindow(flipBuffer, bufIndex++);
			if(p1.compare(comparable))
			{
//				printBufferToCoins(flipBuffer);
				return true;
			}
			else if(p2.compare(comparable))
			{
//				printBufferToCoins(flipBuffer);
				return false;
			}
			
			flipBuffer.add(random.nextBoolean());
		}
	}
	
	private void printBufferToCoins(ArrayList<Boolean> list)
	{
		String str = "";
		for(Boolean bool : list)
		{
			str += bool ? "H" : "T";
		}
		
		System.out.println(str);
	}
	
	private Boolean[] getBufferWindow(ArrayList<Boolean> list, int startIndex)
	{
		Boolean[] array = new Boolean[3];
		int ai = 0;
		for(int i = startIndex; i < startIndex+3; i++)
		{
			array[ai++] = list.get(i);
		}
		return array;
	}
	
	/*
	 * returns the coin choice that is more probable to occur
	 * than the users choice.
	 */
	private Coins getPlayer2Choice(Coins p1)
	{
		String p2Coins = "";
		
		// take the middle coin, copy it, flip it, and make it the first coin of choice.
		p2Coins += p1.sideDown(1);
		
		// coin 2 and coin 3 of player 2 will be coin 1 and coin 2 of player 1
		p2Coins += p1.sideUp(0);
		p2Coins += p1.sideUp(1);
		
		return new Coins(p2Coins);
	}

}
