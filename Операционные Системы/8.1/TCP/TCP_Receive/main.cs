using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace TCP
{
	class Receive
	{
		private string data;

		public Receive(int port)
		{
			TcpClient client = new TcpClient(IPAddress.Loopback.ToString(), port);
			NetworkStream ns = client.GetStream();
			this.data = Message(ns);

			client.Close();
		}

		private string Message(NetworkStream ns)
		{
			byte[] bytes = new byte[1024];
			int bytesRead = ns.Read(bytes, 0, bytes.Length);

			try {
				Console.WriteLine("Сообщение принято.");
				return Encoding.ASCII.GetString(bytes, 0, bytesRead);
			} catch(Exception e) {
				Console.WriteLine(e.ToString());
				return "NULL";
			}
		}

		/*Здесь выводится полученное сообщение*/
		public void Print_Data()
		{
			Console.WriteLine(this.data);
		}

		~Receive()
		{

		}
	}

	class Program
	{
		static void Main()
		{
			Receive receiver = new Receive(1337);
			receiver.Print_Data();
			Console.ReadKey();
		}
	}
}
