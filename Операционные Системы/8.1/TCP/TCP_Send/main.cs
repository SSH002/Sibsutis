using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace TCP
{
	class Send
	{
		private TcpListener listener;

		public Send(int port, string data)
		{
			listener = new TcpListener(IPAddress.Loopback, port);
			listener.Start();  
  
			while (true) {  
				Console.Write("Ожидание соединения...");  
				TcpClient client = listener.AcceptTcpClient();  
  
				Console.WriteLine("Соединение установлено!");  
				NetworkStream ns = client.GetStream();  
  
				byte []byteTime = Encoding.ASCII.GetBytes(data);  
  
				try {
					ns.Write(byteTime, 0, byteTime.Length);  
					ns.Close();  
					client.Close();  
				} catch (Exception e) {  
					Console.WriteLine(e.ToString());  
				}
			}  
		}

		~Send()
		{
			Console.WriteLine("Сообщение отправлено.");
			listener.Stop();
		}
	}

	class Program
	{
		static void Main()
		{
			new Send(1337, "Gendo Ikari");
		}
	}
}
