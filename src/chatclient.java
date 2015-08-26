import java.lang.Process;
import java.net.Socket;
import java.io.Console;
import java.io.DataOutputStream;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class chatclient
{
	public static void main(String[] args)
	{
		int serverPort = 0;
		String ipAddress = "";
		String handle = "";
		String inputString ="";
		Console console = System.console();

		if(args.length != 2) {
			System.out.print("Incorrect syntax! Use \"java chatclient <server host name> <port number>\"");
			System.exit(1);	
		} else {
			ipAddress = args[0];
			serverPort = Integer.parseInt(args[1]);
		}

		try {
			Socket clientSocket = new Socket(ipAddress, serverPort);
			System.out.printf("Connected to server at %s at port %d\n", ipAddress, serverPort);

			boolean tmp = true;
			while(tmp) {
				handle = console.readLine("What\'s your name? ");
				if(handle.length() > 0 && handle.length() < 11) {
					Pattern pattern = Pattern.compile("\\s");
					Matcher matcher = pattern.matcher(handle);
					boolean found = matcher.find();
					if(found) {
						System.out.println("No whitespace allowed");
						continue;
					}

					break;
				} else {
					System.out.println("Username is at most 10 characters");
					continue;
				}
			}
			while(tmp) {
				System.out.printf("%s> ", handle);
				inputString = console.readLine("");
				String tmpString = "";
				if(inputString.equals("\\quit")) {
					tmpString = inputString;
				} else {
					tmpString = handle + "> " + inputString;
				}

				DataOutputStream messageOut = new DataOutputStream(clientSocket.getOutputStream());
				PrintWriter out = new PrintWriter(messageOut, true);

				out.println(tmpString);
				BufferedReader messageIn = new BufferedReader(
				new InputStreamReader(clientSocket.getInputStream()));
				if(inputString.equals("\\quit")) {
					System.out.println("Client closing connection...");
					clientSocket.close();
					System.exit(0);
				}

				inputString = messageIn.readLine();

				if(inputString.equals("\\quit")) {
					System.out.println("Server closing connection...");
					clientSocket.close();
					System.exit(0);
				}

				System.out.println(inputString);

			}
		} catch(Exception e) {
			System.out.println("Could not connect to server");
			System.exit(1);
		}
	}
}