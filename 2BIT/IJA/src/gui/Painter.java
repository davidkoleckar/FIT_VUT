
package gui;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import static java.lang.Thread.sleep;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JOptionPane;

/**
 * Třída realizující vykreslení hrací desky
 * @author Ondrej Janota - xjanot02
 */
public class Painter extends Thread implements Runnable{

        gui g;
        /**
		 * Konstruktor 
		 * @param game 
		 */
        public Painter(gui game){
            super();
            g = game;
        }
        
        @Override
        public void run() {
            
            try {
                
                sleep(200);
            } catch (InterruptedException ex) {
                Logger.getLogger(Painter.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            g.bUndo.addActionListener((ActionEvent e) -> {
                doUndo(e);
            });

            while(!g.game.getGame().getEnd()){

                if (!g.isVisible())
                {
                    break;
                }
                
                System.out.print("");
                
                
               
                
                
                if (g.panels[g.game.getGame().getBoard().getRules().getSize()][g.game.getGame().getBoard().getRules().getSize()] != null)
                {
                    //g.game.getGame().getBoard().getField(0, 0);
                    
                    greenPaint();
                    
                    System.out.print("");
                    
                    for(int i = 1; i < g.game.getGame().getBoard().getRules().getSize() + 1; i++)
                        for(int j = 1; j < g.game.getGame().getBoard().getRules().getSize() + 1; j++)
                        {
                            if (g.game.getGame().getBoard().getField(i, j).getDisk() != null)
                            {
                                orangePaint();

                                if (g.game.getGame().getBoard().getField(i, j).getDisk().isWhite())
                                {
                                    g.panels[i][j].setIcon(g.im2);
                                }
                                else
                                {
                                    g.panels[i][j].setIcon(g.im);
                                }
                            }
                            
                            greenPaint();

                            g.doText();
                        }
                }
                
                if (g.gisComp && g.game.getGame().currentPlayer().isWhite())
                {
                    System.out.print("");

                    blackPaint();
                    
                    orangePaint();
                    
                    g.game.getGame().automaticTurn();
                    
                    orangePaint();
                    
                    g.game.getGame().nextPlayer();
                    
                    //blackPaint();
                    
                    
                    try {
                        sleep(1000);
                        orangePaint();
                    } catch (InterruptedException ex) {
                        Logger.getLogger(Painter.class.getName()).log(Level.SEVERE, null, ex);
                    }
                   
                }
                
                if (!g.game.getGame().currentPlayer().canPutAnywhere())
                {
                    orangePaint();
                    
                    g.game.getGame().nextPlayer();
                    
                    if (g.game.getGame().currentPlayer().canPutAnywhere())
                    {
                        whitePaint();
                        blackPaint();
                        JOptionPane.showMessageDialog(g.dial, g.game.getGame().getNextPlayer() + " can't move");
                        g.doText();
                    }
                    else
                    {
                        g.game.getGame().setEnd();
                        
                        whitePaint();
                        blackPaint();
                        g.doText();
                        
                        if (g.game.getGame().getDisksBlack() + g.game.getGame().getDisksWhite() != g.gsize*g.gsize)
                        {
                            if (g.game.getGame().getDisksBlack() > g.game.getGame().getDisksWhite())
                                g.game.getGame().setDisks(false, g.gsize*g.gsize - g.game.getGame().getDisksWhite());
                            else
                                g.game.getGame().setDisks(true, g.gsize*g.gsize - g.game.getGame().getDisksBlack());
                        }

                        if (g.game.getGame().getDisksBlack() > g.game.getGame().getDisksWhite())
                            JOptionPane.showMessageDialog(g.dial, "Black won");
                        else
                            if (g.game.getGame().getDisksBlack() != g.game.getGame().getDisksWhite())
                                if (g.gisComp)
                                    JOptionPane.showMessageDialog(g.dial, "Computer won");
                                else
                                    JOptionPane.showMessageDialog(g.dial, "White won");
                            else
                                JOptionPane.showMessageDialog(g.dial, "Draw");

                        String[] options = {"Yes, please", "No, thanks"};
                        int n = JOptionPane.showOptionDialog(g.dial, "Play new game?", "Do you want..", JOptionPane.YES_NO_OPTION, JOptionPane.NO_OPTION, null, options, options[1]);
                        if (n == 0)
                        {
                            g.newGame();
                        }
                        else
                            g.exit();
                    }
                }
                    
            }        
        }
    /**
	 * Funkce pro vykresleni cernych disku 
	 */   
    private void blackPaint(){
        for(int i = 1; i < g.game.getGame().getBoard().getRules().getSize() + 1; i++)
            for(int j = 1; j < g.game.getGame().getBoard().getRules().getSize() + 1; j++)
            {
                if (g.game.getGame().getBoard().getField(i, j).getDisk() != null)
                {
                    if (!g.game.getGame().getBoard().getField(i, j).getDisk().isWhite())
                        g.panels[i][j].setIcon(g.im);
                }
            }     
    }
    /**
	 * Funkce pro vykresleni zamrzlych disku 
	 */    
    private void orangePaint(){
        for(int i = 1; i < g.game.getGame().getBoard().getRules().getSize() + 1; i++)
            for(int j = 1; j < g.game.getGame().getBoard().getRules().getSize() + 1; j++)
            {
                if (g.game.getGame().getBoard().getField(i, j).getDisk() != null)
                {
                    if (g.game.getGame().getBoard().getField(i, j).getDisk().isFrozen())
                        g.panels[i][j].setBackground(Color.orange);
                    else
                        g.panels[i][j].setBackground(g.gcolour);
                }
            }

    }
    /**
	 * Funkce pro vykresleni moznych tahu 
	 */ 
    private void greenPaint(){
        for(int i = 1; i < g.game.getGame().getBoard().getRules().getSize() + 1; i++)
            for(int j = 1; j < g.game.getGame().getBoard().getRules().getSize() + 1; j++)
            {
                if (g.game.getGame().getBoard().getField(i, j).getDisk() == null)
                    if (!g.game.getGame().currentPlayer().canPutDisk(g.game.getGame().getBoard().getField(i, j)))
                        g.panels[i][j].setBackground(g.gcolour);
            }
    }
    /**
	 * Funkce pro vykresleni bílích disků 
	 */ 
    private void whitePaint(){
        for(int i = 1; i < g.game.getGame().getBoard().getRules().getSize() + 1; i++)
            for(int j = 1; j < g.game.getGame().getBoard().getRules().getSize() + 1; j++)
            {
                if (g.game.getGame().getBoard().getField(i, j).getDisk() != null)
                {
                    if (g.game.getGame().getBoard().getField(i, j).getDisk().isWhite())
                        g.panels[i][j].setIcon(g.im2);
                }
            }
    }
    /**
	 * Funkce pro vykresleni kamenu po operaci undo 
	 * @param e ActionEvent
	 */ 
    private void doUndo(ActionEvent e){
        undo();
                for (int i = 1; i < g.gsize +1; i++)
                    for (int j = 1; j < g.gsize+1; j++)
                    {
                        if (g.game.getGame().getBoard().getField(i, j).getDisk() != null)
                        {
                            if (g.game.getGame().getBoard().getField(i, j).getDisk().isFrozen())
                                g.panels[i][j].setBackground(Color.orange);
                            else
                                g.panels[i][j].setBackground(g.gcolour);
                        }
                        
                        if (g.game.getGame().getBoard().getField(i, j).getDisk() != null)
                            if (g.game.getGame().getBoard().getField(i, j).getDisk().isWhite())
                            {
                                //panels[i][j].paintImmediately(panels[i][j].getVisibleRect());
                                g.panels[i][j].setIcon(g.im2);
                            }
                            else
                            {
                                //panels[i][j].paintImmediately(panels[i][j].getVisibleRect());
                                g.panels[i][j].setIcon(g.im);
                            }
                        else
                        {
                            g.panels[i][j].setIcon(null);
                            
                            g.panels[i][j].setBackground(g.gcolour);
                        }
                    }
                
                g.game.getGame().nextPlayer();
                g.game.getGame().nextPlayer();
                
                g.doText();
    }

    private void undo(){

        int count;
        if ((count = g.game.getGame().getNextPlayer().doUndo()) != 0)
            if (g.game.getGame().currentPlayer().isWhite())
            {
                g.game.countDisks(true, g.game.getGame().getDisksWhite() + count);
                g.game.countDisks(false, g.game.getGame().currentPlayer().onBoard());
            }
            else
            {
                g.game.countDisks(false, g.game.getGame().getDisksBlack() + count);
                g.game.countDisks(true, g.game.getGame().getNextPlayer().onBoard());
            }
        else
            if (count != -1)
                return;
        
        if ((count = g.game.getGame().currentPlayer().doUndo()) != 0)
            if (g.game.getGame().currentPlayer().isWhite())
            {
                g.game.countDisks(false, g.game.getGame().getDisksBlack() + count);
                g.game.countDisks(true, g.game.getGame().currentPlayer().onBoard());
            }
            else
            {
                g.game.countDisks(true, g.game.getGame().getDisksWhite() + count);
                g.game.countDisks(false, g.game.getGame().getNextPlayer().onBoard());
            }
        else
            if (count != -1)
                g.game.getGame().nextPlayer();
            else
                if (!g.game.getGame().currentPlayer().canPutAnywhere())
                    g.game.getGame().nextPlayer();
    }
}