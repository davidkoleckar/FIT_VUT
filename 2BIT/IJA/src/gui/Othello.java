package gui;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Hlavní okno hry.
 * @author Ondrej Janota - xjanot02
 */
public class Othello extends javax.swing.JFrame{
    
    private JPanel right;
    private JPanel middle;
    private boolean isFreeze;
    
    public Othello() throws IOException{
        super("Othello");
        super.setContentPane(new JLabel(new ImageIcon(getClass().getResource("/images/bg.png"))));
        super.setLayout(new BorderLayout());
        
        
        JPanel left = new JPanel();
        left.setOpaque(false);
        left.setPreferredSize(new Dimension(320, 405));
        left.setLayout(new GridLayout(5, 0, 0, 5));
        
        JButton bComputer = new JButton(new ImageIcon(getClass().getResource("/images/pvspc.png")));
        
        bComputer.setActionCommand("Computer");
        //bNewGame.setPreferredSize(new Dimension(181,74));
        bComputer.setContentAreaFilled(false);
        bComputer.setFocusPainted(false);
        bComputer.setBorderPainted(false);
        
               
        JButton bPlayers = new JButton(new ImageIcon(getClass().getResource("/images/pvsp.png")));
        
        bPlayers.setActionCommand("Two");
        //bNewGame.setPreferredSize(new Dimension(181,74));
        bPlayers.setContentAreaFilled(false);
        bPlayers.setFocusPainted(false);
        bPlayers.setBorderPainted(false);
        
        
        JButton bLoadGame = new JButton(new ImageIcon(getClass().getResource("/images/loadgame.png")));
        bLoadGame.setActionCommand("Load game");
        //bLoadGame.setPreferredSize(new Dimension(240,100));
        bLoadGame.setContentAreaFilled(false);
        bLoadGame.setFocusPainted(false);
        bLoadGame.setBorderPainted(false);
        
        bLoadGame.addActionListener((ActionEvent e) -> 
        {
            click(e);
        });
        
        JButton bHelp = new JButton(new ImageIcon(getClass().getResource("/images/help.png")));
        bHelp.setActionCommand("Help");
        bHelp.setContentAreaFilled(false);
        bHelp.setFocusPainted(false);
        bHelp.setBorderPainted(false);
        
        bHelp.addActionListener((e) -> {
            click(e);
        });
        
        JLabel othello = new JLabel();
        othello.setIcon(new ImageIcon(getClass().getResource("/images/lg.png")));
        othello.setHorizontalAlignment(JLabel.CENTER);
        othello.setPreferredSize(new Dimension(345, 75));

        left.add(bComputer);
        left.add(bPlayers);
        left.add(bLoadGame);
        left.add(bHelp);
        
		//
		//MIDDLE
		//  
        middle = new JPanel();
        middle.setOpaque(false);
        middle.setLayout(new GridLayout(8, 0, 0, 0));
        
		//
		//SIZE
		//  
        JLabel tSize = new JLabel("Size of board");
        tSize.setFont(new Font("Arial", Font.BOLD, 18));
        tSize.setForeground(Color.white);
        
		//
		//ALGORITHM
		//  
        JLabel tAlg = new JLabel("Algorithm");
        tAlg.setFont(new Font("Arial", Font.BOLD, 18));
        tAlg.setForeground(Color.white);
        
		//
		//COLOUR
		//  
        JLabel tColour = new JLabel("Colour");
        tColour.setForeground(Color.white);
        tColour.setFont(new Font("Arial", Font.BOLD, 18));
        
		//
		//FREEZE
		//  
        JLabel tFreeze = new JLabel("Freeze disks");
        tFreeze.setForeground(Color.white);
        tFreeze.setFont(new Font("Arial", Font.BOLD, 18));
        
		//
		//WAIT TIME
		//  
        JLabel tWait = new JLabel("Waiting time");
        tWait.setForeground(Color.white);
        tWait.setFont(new Font("Arial", Font.BOLD, 18));
        tWait.setVisible(false);
        
		//
		//FREEZE TIME
		//  
        JLabel tFreezeWait = new JLabel("Freeze time");
        tFreezeWait.setForeground(Color.white);
        tFreezeWait.setFont(new Font("Arial", Font.BOLD, 18));
        tFreezeWait.setVisible(false);
        
		//
		//COUNT DISK
		//  
        JLabel tDisks = new JLabel("Frozen disks");
        tDisks.setForeground(Color.white);
        tDisks.setFont(new Font("Arial", Font.BOLD, 18));
        tDisks.setVisible(false);
        
		//
		//ADDING INTO PANEL
		//  
        middle.add(tSize);
        middle.add(tAlg);
        middle.add(tColour);
        middle.add(tFreeze);
        middle.add(tWait);
        middle.add(tFreezeWait);
        middle.add(tDisks);
        
        
		//
		//RIGHT PANEL
		//  
        right = new JPanel();
        right.setOpaque(false);
        right.setLayout(new GridLayout(8, 0, 0, 0));
        right.setPreferredSize(new Dimension(150, 40));
        
		//
		//SIZE
		//  
        JPanel rSize = new JPanel();
        rSize.setOpaque(false);
        rSize.setLayout(new FlowLayout(0, 0, 10));
        
        //combo box in the left panel
        String[] opts = {"6", "8", "10", "12"};
        JComboBox boxSize = new JComboBox(opts);
        boxSize.setSelectedIndex(1);
        //boxSize.set nastav doprostred
        boxSize.setPreferredSize(new Dimension(80,25));
        
        rSize.add(boxSize);
        
		//
		//ALOGRITHM
		//  
        JPanel rAlg = new JPanel();
        rAlg.setOpaque(false);
        rAlg.setLayout(new FlowLayout(0, 0, 10));
        
        String[] optsAlg = {"1", "2"};
        JComboBox boxAlg = new JComboBox(optsAlg);
        boxAlg.setSelectedIndex(0);
        boxAlg.setPreferredSize(new Dimension(80, 25));
        
        rAlg.add(boxAlg);
        
		//
		//COLOUR
		//  
        JPanel rColour = new JPanel();
        rColour.setOpaque(false);
        rColour.setLayout(new FlowLayout(0, 0, 10));
        
        String[] colours = {"GREEN", "RED", "BLUE", "YELLOW"};
        JComboBox boxColour = new JComboBox(colours);
        boxColour.setSelectedIndex(0);
        boxColour.setPreferredSize(new Dimension(80, 25));
     
        rColour.add(boxColour);
        
		//
		//FREEZE
		//  
        JPanel rFreeze = new JPanel();
        rFreeze.setOpaque(false);
        rFreeze.setLayout(new FlowLayout(0, 0, 10));
        
        JCheckBox cFreeze = new JCheckBox();
        cFreeze.setSelected(false);
        cFreeze.setPreferredSize(new Dimension(80, 25));
        cFreeze.setOpaque(false);
        
        rFreeze.add(cFreeze);
        
		//
		//WAIT TIME
		//  
        JPanel rWait = new JPanel();
        rWait.setOpaque(false);
        rWait.setLayout(new FlowLayout(0, 0, 10));
        rWait.setVisible(false);

        SpinnerNumberModel snm = new SpinnerNumberModel(2, 0, 10, 1);
        SpinnerModel snm2 = new SpinnerNumberModel(2, 0, 10, 1);
        SpinnerModel snm3 = new SpinnerNumberModel(2, 0, 10, 1);
        JSpinner sWait = new JSpinner(snm);
        sWait.setValue(10);
        sWait.setPreferredSize(new Dimension(80, 25));

        rWait.add(sWait);

		//
		//FREEZE TIME
		//  
        JPanel rFreezeWait = new JPanel();
        rFreezeWait.setOpaque(false);
        rFreezeWait.setLayout(new FlowLayout(0, 0, 10));
        rFreezeWait.setVisible(false);

        JSpinner sFreezeWait = new JSpinner(snm2);
        sFreezeWait.setValue(10);
        sFreezeWait.setPreferredSize(new Dimension(80, 25));
        
        rFreezeWait.add(sFreezeWait);
        
		//
		//COUNT DISK
		//  
        JPanel rDisks = new JPanel();
        rDisks.setOpaque(false);
        rDisks.setLayout(new FlowLayout(0, 0, 10));
        rDisks.setVisible(false);
        
        JSpinner sDisks = new JSpinner(snm3);
        sDisks.setValue(10);
        sDisks.setPreferredSize(new Dimension(80, 25));
        
        rDisks.add(sDisks);
        
		//
		//ADDING INTO RIGHT
		//  
        right.add(rSize);
        right.add(rAlg);
        right.add(rColour);
        right.add(rFreeze);
        right.add(rWait);
        right.add(rFreezeWait);
        right.add(rDisks);
            
        cFreeze.addActionListener((ActionEvent e) -> {
            
            
            if (cFreeze.isSelected())
            {
                tWait.setVisible(true);
                rWait.setVisible(true);
                rFreezeWait.setVisible(true);
                tFreezeWait.setVisible(true);
                tDisks.setVisible(true);
                rDisks.setVisible(true);
                isFreeze = true;
            }
            else
            {
                tWait.setVisible(false);
                rWait.setVisible(false);
                rFreezeWait.setVisible(false);
                tFreezeWait.setVisible(false);
                tDisks.setVisible(false);
                rDisks.setVisible(false);
                isFreeze = false;
            }
        });

		//
		//ADDING INTO OKNO
		//  
        super.add(othello, BorderLayout.PAGE_START);
        super.add(left, BorderLayout.LINE_START);
        super.add(middle, BorderLayout.CENTER);
        super.add(right, BorderLayout.LINE_END);
        
		//
		//ACTIONS
		//  
        bComputer.addActionListener((ActionEvent e) -> 
        {
             click(boxSize, boxColour, boxAlg, true,(int)sWait.getValue(), 
                     (int)sFreezeWait.getValue(), (int)sDisks.getValue(), isFreeze);
        });
        
        bPlayers.addActionListener((ActionEvent e) -> 
        {
             click(boxSize, boxColour, boxAlg, false, (int)sWait.getValue(), 
                     (int)sFreezeWait.getValue(), (int)sDisks.getValue(), isFreeze);
        });
        
        
		//
		//SET OKNO
		//    
        super.pack();
        super.setSize(640,480);
        super.setResizable(false);
        
        super.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        super.setLocationRelativeTo(null);
        super.setVisible(true);
    }
	
    /**
     * Otevre okno Load game
     */    
    private static void loadGame()
    {
        JFrame load = new LoadGame();
    }
    /**
     * Otevre okno s nápovědou
     */  
    private static void help()
    {
        JFrame fGame = new Help();
    }
    
    /**
     * Pro kliknuti na tlacitka
	 * @param e ActionEvent
     */  
    private static void click(ActionEvent e){
        
        if ("Load game".equals(e.getActionCommand()))
            loadGame();
        
        if ("Help".equals(e.getActionCommand()))
            help();
    }
    
    /**
     * Otevre okno s novou hrou
	 * @param size Velikost desky
	 * @param colour Barva desky
	 * @param ui Umělá inteligence
	 * @param isComp Hra proti PC
	 * @param I časový interval
	 * @param B časový interval
	 * @param C počet disků
	 * @param isFreeze Nastavení zamrzani
     */  
    private static void click(JComboBox size, JComboBox colour, JComboBox ui, 
            boolean isComp, int I, int B, int C, boolean isFreeze){
    
        gui game = new gui(
            Integer.parseInt((String)(size.getSelectedItem())),
            (String)(colour.getSelectedItem()),
            Integer.parseInt((String)(ui.getSelectedItem())),
            isComp, null, I, B, C, isFreeze);
        
        Painter p = new Painter(game);
        
        Thread t1 = new Thread(p);
        t1.start();
        
    }
       
}