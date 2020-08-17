package gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedList;
import javax.swing.border.LineBorder;

/**
 * Okno pro nahrání hry s uložené pozice.
 * @author Ondrej Janota - xjanot02
 */
public class LoadGame extends JFrame{
    
    private int countBlack;
    private int countWhite;
    private ArrayList blackDisks;
    private ArrayList whiteDisks;
    private String currentPlayer;
    
    public LoadGame(){
        
        super.setTitle("Load game");
        super.setContentPane(new JLabel(new ImageIcon(getClass().getResource("/images/bg.png"))));
        super.setLayout(new BorderLayout());
        
             
        JLabel textUp = new JLabel("Choose game");
        textUp.setHorizontalAlignment(JLabel.CENTER);
        textUp.setFont(new Font("Arial", Font.BOLD, 36));
        textUp.setForeground(Color.white);
        
        DefaultListModel model = new DefaultListModel();
        
        File fa = new File(getClass().getResource("/saved").getFile());
        File[] list = fa.listFiles();

        for (File list1 : list) {
            if (list1.isFile()) {
                model.addElement(list1.getName());
            }
        }
        
        
        
        JList li = new JList(model);
        li.setPreferredSize(new Dimension(350,200));
        li.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        li.setOpaque(false);
        li.setSelectionBackground(Color.white);
        li.setBackground(Color.LIGHT_GRAY);
        //listScroll.setViewportView(li);
        JScrollPane listScroll = new JScrollPane();
        listScroll.setOpaque(false);
        listScroll.setViewportView(li);
        
        
        
        JPanel c = new JPanel();
        c.setOpaque(false);
        c.setLayout(new FlowLayout(FlowLayout.CENTER, 50, 30)); 
        c.add(listScroll);
        
        JButton bLoad = new JButton(new ImageIcon(getClass().getResource("/images/loadgame.png")));
        bLoad.setActionCommand("Load");
        //bLoad.setPreferredSize(new Dimension(170, 70));
        bLoad.setContentAreaFilled(false);
        bLoad.setFocusPainted(false);
        bLoad.setBorderPainted(false);
        
        JPanel down = new JPanel();
        down.setOpaque(false);
        down.setLayout(new FlowLayout(FlowLayout.CENTER, 50, 30));        
        down.add(bLoad);
        
        super.add(textUp, BorderLayout.PAGE_START);
        super.add(c, BorderLayout.CENTER);
        super.add(down, BorderLayout.PAGE_END);
        
        bLoad.addActionListener((ActionEvent e) -> 
        {
            load(li);
        });
        
        super.pack();
        super.setSize(640,480);
        super.setResizable(false);
        
        //load.setLocationRelativeTo(null);
        super.setVisible(true);
    }
    /**
	 * Provede nahrání hry ze zadaného souboru do hry.
	 * @param l JList zobrazující názvy uložených her.
	 */
    void load(JList l){
        String s = (String) l.getSelectedValue();
        
        if (!s.substring(s.length()-3, s.length()).equals("txt"))
        {
            System.err.println("This is not mine");
            System.exit(1);
        }
          
        
        File f = new File(getClass().getResource("/saved/"+s).getFile());
       
        String text = null;
        
        LinkedList list = new LinkedList();
        
        BufferedReader reader = null;
        
        try {
            reader = new BufferedReader(new FileReader(f));
            

            while ((text = reader.readLine()) != null) {
                list.add(text);
            }
        } catch (FileNotFoundException e) {
            System.err.println("Some special bug, I dont really know, what the f*ck happend");
        } catch (IOException e) {
            System.err.println("This file may get me into trouble.");
        } finally {
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                System.err.println("I really hate this file");
            }
        }
        
        int size = Integer.parseInt((String)list.get(0));
        int alg = Integer.parseInt((String)list.get(2));
        
        gui g = new gui(size, (String)list.get(1),
                alg, Boolean.parseBoolean((String)list.get(3)), f, 2, 2, 2, true);
        
        Painter p = new Painter(g);
        
        Thread t = new Thread(p);
        t.start();

    }
    
    
}
