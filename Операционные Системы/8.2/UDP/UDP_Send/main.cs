using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace UDP
{
	class Send
	{
		private Socket socket;

		public Send(int port, string message)
		{
			socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
			socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, port);
			IPAddress broadcast = IPAddress.Parse(IPAddress.Broadcast.ToString());

			byte []sendbuf = Encoding.ASCII.GetBytes(message);
			IPEndPoint ep = new IPEndPoint(IPAddress.Broadcast, port);
			
			try {
				socket.SendTo(sendbuf, ep);
				socket.Close();
				Console.WriteLine("Сообщение отправлено по широковещательному адресу!");
			} catch {
				Console.WriteLine("Порт [" + port + "] заблокирован!");
			}
		}

		~Send()
		{
			
		}
	}

	class Program
	{
		static void Main()
		{
			new Send(1132, "Gendo Ikari");
			Console.ReadKey();
		}
	}
}
