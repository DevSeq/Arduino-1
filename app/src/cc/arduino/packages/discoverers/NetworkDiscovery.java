package cc.arduino.packages.discoverers;

import cc.arduino.packages.BoardPort;
import cc.arduino.packages.Discovery;
import cc.arduino.packages.discoverers.network.NetworkChecker;
import processing.app.Base;
import processing.app.helpers.PreferencesMap;
import processing.app.zeroconf.jmdns.ArduinoDNSTaskStarter;

import javax.jmdns.*;
import javax.jmdns.impl.DNSTaskStarter;
import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.*;

public class NetworkDiscovery implements Discovery, ServiceListener, cc.arduino.packages.discoverers.network.NetworkTopologyListener {

  private Timer timer;
  private List<BoardPort> ports;
  private final Map<InetAddress, JmDNS> mappedJmDNSs;

  public NetworkDiscovery() {
    DNSTaskStarter.Factory.setClassDelegate(new ArduinoDNSTaskStarter());
    this.ports = new ArrayList<BoardPort>();
    this.mappedJmDNSs = new Hashtable<InetAddress, JmDNS>();
  }

  @Override
  public List<BoardPort> discovery() {
    List<BoardPort> ports = clonePortsList();
    Iterator<BoardPort> iterator = ports.iterator();
    while (iterator.hasNext()) {
      try {
        InetAddress address = Inet4Address.getByName(iterator.next().getAddress());
        if (!address.isReachable(100)) {
          iterator.remove();
        }
      } catch (UnknownHostException e) {
        iterator.remove();
      } catch (IOException e) {
        iterator.remove();
      }
    }
    return ports;
  }

  private List<BoardPort> clonePortsList() {
    synchronized (this) {
      return new ArrayList<BoardPort>(this.ports);
    }
  }

  @Override
  public void setPreferences(PreferencesMap options) {
  }

  @Override
  public void start() throws IOException {
    this.timer = new Timer(this.getClass().getName() + " timer");
    new NetworkChecker(this, NetworkTopologyDiscovery.Factory.getInstance()).start(timer);
  }

  @Override
  public void stop() throws IOException {
    timer.purge();
    // we don't close each JmDNS instance as it's too slow
  }

  @Override
  public void serviceAdded(ServiceEvent serviceEvent) {
    String type = serviceEvent.getType();
    String name = serviceEvent.getName();

    JmDNS dns = serviceEvent.getDNS();

    dns.requestServiceInfo(type, name);
    ServiceInfo serviceInfo = dns.getServiceInfo(type, name);
    if (serviceInfo != null) {
      dns.requestServiceInfo(type, name);
    }
  }

  @Override
  public void serviceRemoved(ServiceEvent serviceEvent) {
    String name = serviceEvent.getName();
    synchronized (this) {
      for (BoardPort port : ports) {
        if (port.getBoardName().equals(name))
          ports.remove(port);
      }
    }
  }

  @Override
  public void serviceResolved(ServiceEvent serviceEvent) {
    ServiceInfo info = serviceEvent.getInfo();
    String address = info.getInet4Addresses()[0].getHostAddress();
    String name = serviceEvent.getName();

    PreferencesMap prefs = null;
    String board = null;
    if (info.hasData()) {
      prefs = new PreferencesMap();
      board = info.getPropertyString("board");
      prefs.put("board", board);
      prefs.put("distro_version", info.getPropertyString("distro_version"));
    }

    String label = name + " at " + address;
    if (board != null) {
      String boardName = Base.getPlatform().resolveDeviceByBoardID(Base.packages, board);
      label += " (" + boardName + ")";
    }

    BoardPort port = new BoardPort();
    port.setAddress(address);
    port.setBoardName(name);
    port.setProtocol("network");
    port.setPrefs(prefs);
    port.setLabel(label);

    synchronized (this) {
      removeDuplicateBoards(port);
      ports.add(port);
    }
  }

  private void removeDuplicateBoards(BoardPort newBoard) {
    Iterator<BoardPort> iterator = ports.iterator();
    while (iterator.hasNext()) {
      BoardPort board = iterator.next();
      if (newBoard.getAddress().equals(board.getAddress())) {
        iterator.remove();
      }
    }
  }

  @Override
  public void inetAddressAdded(InetAddress address) {
    if (mappedJmDNSs.containsKey(address)) {
      return;
    }
    try {
      JmDNS jmDNS = JmDNS.create(address);
      jmDNS.addServiceListener("_arduino._tcp.local.", this);
      mappedJmDNSs.put(address, jmDNS);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  @Override
  public void inetAddressRemoved(InetAddress address) {
    JmDNS jmDNS = mappedJmDNSs.remove(address);
    if (jmDNS != null) {
      try {
        jmDNS.close();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
  }
}