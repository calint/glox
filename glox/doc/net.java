package d3;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.InvocationTargetException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Date;
import java.util.Iterator;
final public class net{
	private final static class plr{
		SocketChannel socketChannel;
		SelectionKey selectionKey;
		int plrnbr;
	}
	public static PrintStream out=System.out;
	public static final int protocol_msg_len=32;
	public static int nplayers=2;
	static int nplayersconnected=0;
	public static void main(String[] args) throws Throwable{
		if(args.length>0&&args[0].equals("?")){
			out.println("example: net 8085 2\nopens server on port 8085 for 2 players");
			return;
		}
		if(args.length>0)
			cfg.net_server_port=args[0];

		final ServerSocketChannel serverSocketChannel=ServerSocketChannel.open();
		serverSocketChannel.configureBlocking(false);
		final InetSocketAddress inetSocketAddress=new InetSocketAddress(Integer.parseInt(cfg.net_server_port));
		final ServerSocket serverSocket=serverSocketChannel.socket();
		serverSocket.bind(inetSocketAddress);
		print_stats(out);
		final Selector selector=Selector.open();
		serverSocketChannel.register(selector,SelectionKey.OP_ACCEPT);

		final plr[]nps=new plr[nplayers];
		for(int n=0;n<nps.length;n++){
			nps[n]=new plr();
		}
		final ByteBuffer[]bbs=new ByteBuffer[nps.length];
		for(int n=0;n<bbs.length;n++){
			bbs[n]=ByteBuffer.allocate(protocol_msg_len);
		}
		int players_that_sent_keys=0;
		int nplayers_that_rec_keys=0;
		while(true)try{
			selector.select();
			final Iterator<SelectionKey>iterator=selector.selectedKeys().iterator();
			while(iterator.hasNext()){
				final SelectionKey selectionKey=iterator.next();
				iterator.remove();
				if(selectionKey.isAcceptable()){
					final plr p=nps[nplayersconnected];
					p.socketChannel=serverSocketChannel.accept();
					p.socketChannel.configureBlocking(false);
					p.selectionKey=p.socketChannel.register(selector,SelectionKey.OP_READ,p);
					p.plrnbr=nplayersconnected;
					nplayersconnected++;
					out.println("player "+p.socketChannel.socket().getInetAddress().getHostAddress()+" connected ("+nplayersconnected+" of "+nps.length+")");
					if(nplayersconnected==nps.length){
						out.println("all players connected");
						serverSocketChannel.register(selector,0);
					}
				}else if(selectionKey.isReadable()){
					final plr p=(plr)selectionKey.attachment();
					p.selectionKey.interestOps(0);
					bbs[p.plrnbr].clear();
					final int n=p.socketChannel.read(bbs[p.plrnbr]);
					if(n==-1)
						return;
					bbs[p.plrnbr].flip();
//						System.out.println(new String(bbs[p.plrnbr].array()));
					players_that_sent_keys++;
					if(players_that_sent_keys==nplayers){
						for(plr np:nps){
							np.selectionKey.interestOps(SelectionKey.OP_WRITE);
						}
						players_that_sent_keys=0;
					}
				}else if(selectionKey.isWritable()){
					final plr p=(plr)selectionKey.attachment();
					p.selectionKey.interestOps(0);
					p.socketChannel.write(bbs);
					for(final ByteBuffer bb:bbs)
						bb.flip();
					nplayers_that_rec_keys++;
					if(nplayers_that_rec_keys==nplayers){
						nplayers_that_rec_keys=0;
						for(plr np:nps){
							np.selectionKey.interestOps(SelectionKey.OP_READ);
						}
//							frameno++;
//							System.out.print(frameno+"\r");
					}
				}
			}
		}catch(final Throwable e){log(e);}
	}
	public static void print_stats(final OutputStream out) throws Throwable{
		final PrintStream ps=new PrintStream(out);
		ps.println("             time: "+new Date(System.currentTimeMillis()));
		ps.println("             port: "+cfg.net_server_port);
		ps.println("          players: "+nplayersconnected+"/"+nplayers);
		final Runtime rt=Runtime.getRuntime();
		if(cfg.gc_before_stats)
			rt.gc();
		long m1=rt.totalMemory();
		long m2=rt.freeMemory();
		ps.println("         ram used: "+((m1-m2)>>10)+" KB");
		ps.println("         ram free: "+(m2>>10)+" KB");
	}
	public static synchronized void log(final Throwable t){
		Throwable e=t;
		if(t instanceof InvocationTargetException)
			e=((InvocationTargetException)t).getCause();
		while(e.getCause()!=null)
			e=e.getCause();
//		if(e instanceof java.nio.channels.CancelledKeyException)
//			return;
//		if(e instanceof java.nio.channels.ClosedChannelException)
//			return;
//		if(e instanceof java.io.IOException){
//			if("Broken pipe".equals(e.getMessage()))
//				return;
//			if("Connection reset by peer".equals(e.getMessage()))
//				return;
//		}
//		//		htp.out.println();
		net.out.println("\n"+net.stackTraceLine(e));
		throw new Error("connection to player lost");
	}
	public static String stackTraceLine(final Throwable e){
		return stackTrace(e).replace('\n',' ').replace('\r',' ').replaceAll("\\s+"," ").replaceAll(" at "," @ ");
	}
	public static String stackTrace(Throwable e){
		if(e==null)
			return null;
		final StringWriter sw=new StringWriter();
		final PrintWriter out=new PrintWriter(sw);
		e.printStackTrace(out);
		out.close();
		return sw.toString();
	}
}
