package gui;

import javax.swing.*;
import java.awt.*;

/**
 * Okno pro výpis nápovědy.
 * @author Ondrej Janota - xjanot02
 */
public class Help extends JFrame{
    
    public Help(){
        super.setTitle("Help");
        super.setContentPane(new JLabel(new ImageIcon(getClass().getResource("/images/bg.png"))));
        super.setLayout(new GridLayout(12, 0, 50, 0));
        super.setForeground(Color.red);
        //JPanel panel = new JPanel();
        //panel.setBackground(Color.red);
        JLabel text1 = new JLabel("Othello je desková hra pro dva hráče  hraná na čtvercové desce o zadané velikosti.");
        text1.setFont(new Font("Arial", Font.BOLD, 16));
        text1.setForeground(Color.white);
        
        JLabel text2 = new JLabel("Hráči na desku pokládají kameny, které jsou buď bílé nebo černé.");
        text2.setFont(new Font("Arial", Font.BOLD, 16));
        text2.setForeground(Color.white);
        
        JLabel text3 = new JLabel("Pokládají je tak, aby mezi právě položený kámen a jiný kámen své barvy");
        text3.setFont(new Font("Arial", Font.BOLD, 16));
        text3.setForeground(Color.white);
        
        JLabel text4 = new JLabel("uzavřeli souvislou řadu soupeřových kamenů.");
        text4.setFont(new Font("Arial", Font.BOLD, 16));
        text4.setForeground(Color.white);
        
        JLabel text5 = new JLabel("Tyto kameny se potom otočí a stanou se kameny druhého hráče.");
        text5.setFont(new Font("Arial", Font.BOLD, 16));
        text5.setForeground(Color.white);
        
        JLabel text6 = new JLabel("Vítězí hráč, který po zaplnění desky na ní má více svých kamenů.");
        text6.setFont(new Font("Arial", Font.BOLD, 16));
        text6.setForeground(Color.white);
        
        super.add(text1);
        super.add(text2);
        super.add(text3);
        super.add(text4);
        super.add(text5);
        super.add(text6);
        
        //fGame.add(panel);
        
        super.pack();
        super.setSize(640,480);
        super.setResizable(false);
        
        /*Hráči na desku pokládají kameny, které jsou z jedné strany bílé a z druhé černé, tak, aby mezi právě položený kámen a jiný kámen své barvy uzavřeli souvislou řadu soupeřových kamenů; tyto kameny se potom otočí a stanou se kameny druhého hráče. Vítězí hráč, který po zaplnění desky na ní má více svých kamenů.");*/
        super.setVisible(true);
        
    }
}
