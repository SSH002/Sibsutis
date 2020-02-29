using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace UDP
{
	class Receive
	{
		private UdpClient listener;

		public Receive(int port)
		{
			listener = new UdpClient(port);
			IPEndPoint groupEP = new IPEndPoint(IPAddress.Broadcast, port);

			try {
				while (true) {
					byte []bytes = listener.Receive(ref groupEP);
					Console.WriteLine("Получено сообщение:");
					Console.WriteLine(Encoding.ASCII.GetString(bytes, 0, bytes.Length));
				}
			} catch (SocketException e) {
				Console.WriteLine(e);		/*Здесь выводится полученное сообщение*/
			}
		}

		~Receive()
		{
			listener.Close();
		}
	}

	class Program
	{
		static void Main()
		{
			Receive receiver = new Receive(1132);
			Console.ReadKey();
		}
	}
}
