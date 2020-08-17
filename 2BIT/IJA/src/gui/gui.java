package gui;

import javax.swing.*;
import java.awt.*;
import game.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.border.LineBorder;

/**
 * Okno zobrazující desku hry
 * @author Ondrej Janota - xjanot02
 */
public class gui extends JFrame implements Runnable{
    
    private final int gAlg;
    protected newGame game;
    protected int gsize;
    protected Color gcolour;
    protected boolean gisComp;
    protected boolean gfreeze;

    protected ImageIcon im;
    protected ImageIcon im2;
    protected JLabel[][] panels = null;
    private JPanel left;
    protected JLabel tBlack;
    protected JLabel tWhite;
    protected JLabel tActualWhite;
    protected JLabel tActualBlack;
    protected JDialog dial;
    protected JButton bUndo;
    //int i = 0;
    private int I;
    private int B;
    private int C;
    
    private boolean isChanged;
    protected boolean canPaint = false;
    protected boolean canSleep = false;
    private Thread t;
    /**
	 * Konstruktor
	 * @param size Velikost desky
	 * @param colour Barva desky
	 * @param ui Umělá inteligence
	 * @param isComp Hra proti PC
	 * @param I časový interval
	 * @param B časový interval
	 * @param C počet disků
	 * @param isFreeze Nastavení zamrzani
	 */
    public gui(int size, String colour, int alg, boolean isComp, File load, 
            int I, int B, int C, boolean freeze){
        
        super("Othello");
        
        if (size < 10)
            super.setContentPane(new JLabel(new ImageIcon(getClass().getResource("/images/bg.png"))));
        else
            super.setContentPane(new JLabel(new ImageIcon(getClass().getResource("/images/bg10.png"))));
        super.setLayout(new BorderLayout());
        
        this.im = new ImageIcon(getClass().getResource("/images/"+size+"b.png"));
        this.im2 = new ImageIcon(getClass().getResource("/images/"+size+"w.png"));
        
        if (isComp)
            this.gAlg = alg;
        else
            this.gAlg = 0;
        
        this.gfreeze = freeze;
        
        this.gsize = size;
        
        switch (colour) {
            case "GREEN":
                gcolour = Color.GREEN;
                break;
            case "RED":
                gcolour = Color.RED;
                break;
            case "YELLOW":
                gcolour = Color.YELLOW;
                break;
            case "BLUE":
                gcolour = Color.CYAN;
                break;
            default:
                gcolour = Color.GREEN;
        }
        
        this.gisComp = isComp;
        
        this.I = I;
        this.B = B;
        this.C = C;
        
        System.out.print(I + " " + B + " " + C);
        
        this.isChanged = false;
        
        //this.panels = new JPanel[size+1][size+1];
        this.panels = new JLabel[size+1][size+1];
        this.game = new newGame(size, isComp, this.gAlg);
        
        LinkedList list = new LinkedList();
        
        
        
        if (load != null)
        {
            String text = null;
            
            BufferedReader reader = null;
        
            try {
                reader = new BufferedReader(new FileReader(load));

                while ((text = reader.readLine()) != null)
                    list.add(text);
            }
            catch (FileNotFoundException e) {
                System.err.println("NOT EXIST");
                System.exit(1);
            }
            catch (IOException e) {
                System.err.println("CANNOT READ");
                System.exit(1);
            }
            finally {
                try {
                    if (reader != null)
                        reader.close();
                }
                catch (IOException e) {
                    System.err.println("CANNOT CLOSE");
                    System.exit(1);
                }
            }
            
            int index = 4;
        
            String currentPlayer = (String)list.get(index++);
            
            boolean isWhite;
            
            isWhite = currentPlayer.equals("WHITE");
        
            int countBlack = Integer.parseInt((String)list.get(index++));

            ArrayList blackDisks = new ArrayList<String>();

            for(int i = index; i < countBlack + index; i++)
                blackDisks.add(i - index, (String)list.get(i));

            index += countBlack;

            ArrayList whiteDisks = new ArrayList<String>();

            int countWhite = Integer.parseInt((String)list.get(index++));

            for(int i = index; i < countWhite + index; i++)
                whiteDisks.add(i - index, (String)list.get(i));
            
            index += countWhite;
        
            this.game.currentPlayer(isWhite);
            this.game.countDisks(false, countBlack);
            this.game.countDisks(true, countWhite);
            this.game.disks(false, blackDisks);
            this.game.disks(true, whiteDisks);
            
            LinkedList remove = new LinkedList();
            LinkedList turned = new LinkedList();
            
            remove = game.getGame().currentPlayer().getRemove();
            turned = game.getGame().currentPlayer().getTurned();
            
            int removeSize = Integer.parseInt((String)list.get(index++));
            
            for (int i = index; i < removeSize + index; i++)
                remove.add((String)list.get(i));
            
            index += removeSize;
            
            int turnedSize = Integer.parseInt((String)list.get(index++));
            
            for (int i = index; i < turnedSize + index; i++)
                turned.add((String)list.get(i) + System.lineSeparator());
            
            index += turnedSize;
            
            
            remove = game.getGame().getNextPlayer().getRemove();
            turned = game.getGame().getNextPlayer().getTurned();
            
            removeSize = Integer.parseInt((String)list.get(index++));
            
            for (int i = 0; i < removeSize; i++)
                remove.add((String)list.get(index++));
            
            turnedSize = Integer.parseInt((String)list.get(index++));
            
            for (int i = 0; i < turnedSize; i++)
                turned.add((String)list.get(index++) + System.lineSeparator());
            
            game.getGame().getNextPlayer().setRemove(remove);
            game.getGame().getNextPlayer().setTurned(turned);
            
            
            gfreeze = Boolean.parseBoolean((String)list.get(index++));
            
            this.I = Integer.parseInt((String)list.get(index++));
            
            this.B = Integer.parseInt((String)list.get(index++));
            
            this.C = Integer.parseInt((String)list.get(index++));
            
            
            if (isComp && this.game.getGame().currentPlayer().isWhite())
            {
                this.game.getGame().automaticTurn();
                this.game.getGame().nextPlayer();
            }
        }
        
        if (gfreeze)
            game.getGame().doFreeze(this.I,this.B, this.C);
        
        JPanel up = new JPanel();
        up.setOpaque(false);
        up.setLayout(new GridLayout(0, 5, 0, 0));
        
        JPanel upOne = new JPanel();
        upOne.setOpaque(false);
        upOne.setLayout(new FlowLayout(FlowLayout.CENTER, 40, 10));
   
        tBlack = new JLabel("BLACK");
        tBlack.setFont(new Font("Arial", Font.BOLD, 25));
        tBlack.setForeground(Color.white);
        tBlack.paintImmediately(tBlack.getVisibleRect());
        
        JPanel upTwo = new JPanel();
        upTwo.setOpaque(false);
        upTwo.setLayout(new FlowLayout(FlowLayout.CENTER, 40, 10));
        
        tActualBlack = new JLabel("");
        tActualBlack.setMinimumSize(new Dimension(140,30));
        tActualBlack.setMaximumSize(new Dimension(140, 30));
        tActualBlack.setPreferredSize(new Dimension(140, 30));
        tActualBlack.setHorizontalAlignment(JLabel.CENTER);
        tActualBlack.setText(String.valueOf(game.getGame().getDisksBlack()));
        tActualBlack.setFont(new Font("Arial", Font.BOLD, 25));
        tActualBlack.setForeground(Color.white);
        
        upOne.add(tBlack);
        upTwo.add(tActualBlack);
        
        JPanel upThree = new JPanel();
        upThree.setOpaque(false);
        upThree.setLayout(new FlowLayout(FlowLayout.CENTER, 40, 10));
        
        JLabel pomlcka = new JLabel(":");
        pomlcka.setFont(new Font("Arial", Font.BOLD, 25));
        pomlcka.setForeground(Color.white);
        
        upThree.add(pomlcka);
        
        JPanel upFour = new JPanel();
        upFour.setOpaque(false);
        upFour.setLayout(new FlowLayout(FlowLayout.CENTER, 40, 10));
        
        tActualWhite = new JLabel();
        tActualWhite.setText(String.valueOf(game.getGame().getDisksWhite()));
        tActualWhite.setFont(new Font("Arial", Font.BOLD, 25));
        tActualWhite.setForeground(Color.white);
        tActualWhite.setBounds(tActualWhite.getVisibleRect());
        
        
        JPanel upFive = new JPanel();
        upFive.setOpaque(false);
        upFive.setLayout(new FlowLayout(FlowLayout.CENTER, 40, 10));
        
        tWhite = new JLabel("WHITE");
        tWhite.setFont(new Font("Arial", Font.BOLD, 25));
        tWhite.setForeground(Color.white);
        tWhite.paintImmediately(tWhite.getVisibleRect());
        
        upFour.add(tActualWhite);
        upFive.add(tWhite);
        
        up.add(upOne);
        up.add(upTwo);
        up.add(upThree);
        up.add(upFour);
        up.add(upFive);
        
        
        left = new JPanel();
        left.setOpaque(false);
        left.setLayout(new GridLayout(size, size, 0, 0));
        
        JPanel right = new JPanel();
        right.setOpaque(false);
        right.setLayout(new GridLayout(2, 0, 0, 80));

        if(size == 6 || size == 8){
            up.setPreferredSize(new Dimension(640, 50));
            right.setPreferredSize(new Dimension(210, 430));
            left.setPreferredSize(new Dimension(430,430));
            super.setSize(new Dimension(640, 480));
        }
        else
        {
            up.setPreferredSize(new Dimension(800, 50));
            right.setPreferredSize(new Dimension(250, 550));
            left.setPreferredSize(new Dimension(550, 550));
            super.setSize(new Dimension(800, 600));
        }
        
        JPanel rightUp = new JPanel();
        rightUp.setLayout(new FlowLayout(1));
        rightUp.setOpaque(false);

        JPanel rightDown = new JPanel();
        rightDown.setLayout(new GridLayout(3, 0, 0, 10));
        rightDown.setOpaque(false);

        JButton bSaveGame = new JButton(new ImageIcon(getClass().getResource("/images/savegame.png")));
        bSaveGame.setActionCommand("Save game");
        bSaveGame.setContentAreaFilled(false);
        bSaveGame.setFocusPainted(false);
        bSaveGame.setBorderPainted(false);
        
        bSaveGame.addActionListener((ActionEvent e) -> {
            click(e);
        });

        bUndo = new JButton(new ImageIcon(getClass().getResource("/images/undo.png")));
        bUndo.setActionCommand("Undo");
        bUndo.setContentAreaFilled(false);
        bUndo.setFocusPainted(false);
        bUndo.setBorderPainted(false);
         
        dial = new JDialog();
        
        dial.setLocationRelativeTo(left);
        dial.setVisible(false);
        
        boolean once = false;
        
        for(int i = 1; i < gsize + 1; i++)
            for(int j = 1; j < gsize + 1; j++){
                JLabel field = new JLabel();
                field.setOpaque(true);
                field.setBorder(new LineBorder(Color.BLACK));
                field.setHorizontalAlignment(JLabel.CENTER);
                
                field.setBackground(gcolour);
                        
                panels[i][j] = field;
                left.add(field);
                
                
                
                field.addMouseListener(new MouseListenerImpl(field));
            }
        
        
        rightDown.add(bSaveGame);
        rightDown.add(bUndo);
        
        right.add(rightUp);
        right.add(rightDown);
        
        super.add(up, BorderLayout.PAGE_START);
        super.add(left, BorderLayout.CENTER);
        super.add(right, BorderLayout.LINE_END);
        
        super.setResizable(false);
        super.setLocation((new Random()).nextInt(500), (new Random()).nextInt(200));
        super.setVisible(true);
        
        
        
        super.addWindowListener(new WindowListener() {
            @Override
            public void windowOpened(WindowEvent e) {
                if (game.getGame().currentPlayer().isWhite())
                    tWhite.setForeground(Color.yellow);
                else
                    tBlack.setForeground(Color.yellow);
            }

            @Override
            public void windowClosing(WindowEvent e) {
            }

            @Override
            public void windowClosed(WindowEvent e) {
            }

            @Override
            public void windowIconified(WindowEvent e) {
                if (game.getGame().currentPlayer().isWhite())
                    tWhite.setForeground(Color.yellow);
                else
                    tBlack.setForeground(Color.yellow);
            }

            @Override
            public void windowDeiconified(WindowEvent e) {
            }

            @Override
            public void windowActivated(WindowEvent e) {
                if (game.getGame().currentPlayer().isWhite())
                    tWhite.setForeground(Color.yellow);
                else
                    tBlack.setForeground(Color.yellow);
            }

            @Override
            public void windowDeactivated(WindowEvent e) {
            }
        });
    }
    /**
	 * Zavření okna 
	 */
    protected void exit(){
        super.dispose();
    }
    /**
	 * Zavření okna a otevření nové
	 */    
    protected void newGame(){
        super.dispose();
        gui g = new gui(gsize, gcolour.toString().toUpperCase(), gAlg, gisComp, null, I, B, C, gfreeze);
        Painter p = new Painter(g);
        t = new Thread(p);
        t.start();
    }
    /**
	 * Akce po kliknu na save game
     * @param e ActionEvent	 
	 */    
    protected void click(ActionEvent e){
        if (e.getActionCommand().equals("Save game"))
            saveGame();
    }
    /**
	 * Provede uložení rozehrané hry 
	 */    
    private void saveGame(){
        String name = JOptionPane.showInputDialog(dial, "Name of game:");
        
        if (name == null)
            return;
        
        name = name + ".txt";
        
        File f = new File(getClass().getResource("/saved/").getPath() + name);
        
        if (f.exists()){
            String[] options = {"Yes", "No"};
            int n = JOptionPane.showOptionDialog(dial, "Do you want overwrite it?", "This game already exists!", JOptionPane.YES_NO_OPTION, JOptionPane.NO_OPTION, null, options, options[1]);
            if (n != 0)
                return;
        }
        else
            try {
                f.createNewFile();
            } catch (IOException ex) {
                System.err.println("FILE NEVER BE BORNED");
            }
        
        String save;
        save = gsize + System.lineSeparator();
        save += gcolour + System.lineSeparator();
        save += gAlg + System.lineSeparator();
        save += gisComp + System.lineSeparator();
        save += game.getGame().currentPlayer().toString() + System.lineSeparator();
        save += game.getGame().getDisksBlack() + System.lineSeparator();
        
        ArrayList disks = game.getDisks(false);
        
        for (int i = 0; i < game.getGame().getDisksBlack(); i++)
            save += disks.get(i) + System.lineSeparator();
        
        save += game.getGame().getDisksWhite() + System.lineSeparator();
        
        disks = game.getDisks(true);
        
        for (int i = 0; i < game.getGame().getDisksWhite(); i++)
            save += disks.get(i) + System.lineSeparator();
        
        LinkedList remove = new LinkedList();
        
        remove = game.getGame().currentPlayer().getRemove();
        
        LinkedList turned = new LinkedList();
        turned = game.getGame().currentPlayer().getTurned();
        
        save += remove.size() + System.lineSeparator();
        
        int rSize = remove.size();
        
        for (int i = 0; i < rSize; i++)
            save += remove.get(i) + System.lineSeparator();
        
        save += turned.size() + System.lineSeparator();
        
        int tSize = turned.size();
        
        for (int i = 0; i < tSize; i++)
            save += turned.get(i);
        
        remove = game.getGame().getNextPlayer().getRemove();
        turned = game.getGame().getNextPlayer().getTurned();
        
        save += remove.size() + System.lineSeparator();
        
        rSize = remove.size();
        
        for (int i = 0; i < rSize; i++)
            save += remove.get(i) + System.lineSeparator();

        save += turned.size() + System.lineSeparator();
        
        tSize = turned.size();
        
        for (int i = 0; i < tSize; i++)
            save += turned.get(i);
        
        save += Boolean.toString(gfreeze).toUpperCase() + System.lineSeparator();
        
        save += I + System.lineSeparator();
        
        save += B + System.lineSeparator();
        
        save += C + System.lineSeparator();
        
        FileWriter fw = null;
        
        try {
            fw = new FileWriter(f.getAbsoluteFile());
        } catch (IOException ex) {
            System.err.println("FILE IS STRONGER AS I AM.");
            System.exit(1);
        }
        
        BufferedWriter bw = new BufferedWriter(fw); 
        
        try {
            bw.write(save);
        } catch (IOException ex) {
            System.err.println("FILE MUST HAVE REALLY COOL FIREWALL");
            System.exit(1);
        }
       
        try {
            bw.close();
        } catch (IOException ex) {
            System.err.println("SOMETIMES FILE IS SO STRONG.. THAT CANNOT CLOSE");
            System.exit(1);
        }
    }

    @Override
    public void run() {
        
        for(int i = 1; i < gsize + 1; i++)
            for(int j = 1; j < gsize + 1; j++){
                JLabel field = new JLabel();
                field.setOpaque(true);
                field.setBorder(new LineBorder(Color.BLACK));
                
                field.setBackground(gcolour);
                        
                panels[i][j] = field;
                left.add(field);
                
                field.addMouseListener(new MouseListenerImpl(field));
            }
    }

	/**
	 * Třída MouseListener
	 * @author Ondrej Janota - xjanot02
	 */
	private class MouseListenerImpl implements MouseListener {

        private final JLabel field;
        private ImageIcon imT;
        private boolean once;
        
        public MouseListenerImpl(JLabel field) {
            this.field = field;
            this.once = false;
        }

        @Override
        public void mouseClicked(MouseEvent e) {
        }

        @Override
        public void mousePressed(MouseEvent e) {
            for (int i = 1; i < gsize +1; i++)
                for (int j = 1; j < gsize + 1; j++)
                    if (panels[i][j].equals(field))
                        if (game.getGame().currentPlayer().putDisk(game.getGame().getBoard().getField(i, j)))
                        {
                            if (game.getGame().getDisksBlack() + game.getGame().getDisksWhite() > C && !isChanged)
                            {
                                game.getGame().setFreeze(true);
                                isChanged = true;
                            }
                            
                            game.getGame().nextPlayer();
                        }
        }

        @Override
        public void mouseReleased(MouseEvent e) {
        }

        @Override
		/**
		 * Funkce pro najeti mysi na dane policko hraci desky
		 * @param e MouseEvent
		 */
        public void mouseEntered(MouseEvent e) {
            if (!this.once)
            {
                this.imT = new ImageIcon(getClass().getResource("/images/pruhledna.png"));
                this.imT = new ImageIcon(imT.getImage().getScaledInstance(this.field.getWidth()-7, this.field.getHeight()-7, Image.SCALE_DEFAULT));
                this.once = true;
            }
            for(int i = 1; i < gsize + 1; i++)
                for (int j = 1; j < gsize + 1; j++)
                    if (panels[i][j].equals(field))
                        if (game.getGame().currentPlayer().canPutDisk(game.getGame().getBoard().getField(i, j)))
                            panels[i][j].setIcon(this.imT);
        }

        @Override
		/**
		 * Funkce po odjeti mysi z daneho policka hraci desky
		 * @param e MouseEvent
		 */
        public void mouseExited(MouseEvent e){
            for(int i = 1; i < gsize + 1; i++)
                for (int j = 1; j < gsize + 1; j++)
                    if (panels[i][j].equals(field))
                        if (game.getGame().currentPlayer().canPutDisk(game.getGame().getBoard().getField(i, j)))
                            panels[i][j].setIcon(null);
            
        }

        private void paintComponent() {
            for(int i = 1; i < game.getGame().getBoard().getRules().getSize() + 1; i++)
                for(int j = 1; j < game.getGame().getBoard().getRules().getSize() + 1; j++)
                {
                    if (game.getGame().getBoard().getField(i, j).getDisk() != null)
                    {
                        if (!game.getGame().getBoard().getField(i, j).getDisk().isWhite())
                            panels[i][j].setIcon(im);
                    }
                }
            doText();
        }
    }
	/**
	 * Funkce pro aktualizaci hráče na tahu a skore hry
	 */
    protected void doText(){
            if(game.getGame().currentPlayer().isWhite())
            {
                tBlack.setForeground(Color.white);
                tWhite.setForeground(Color.yellow);
            }
            else
            {
                tWhite.setForeground(Color.white);
                tBlack.setForeground(Color.yellow);
            }
            
            tActualBlack.setText(String.valueOf(game.getGame().getDisksBlack()));
            tActualWhite.setText(String.valueOf(game.getGame().getDisksWhite()));
        }
}