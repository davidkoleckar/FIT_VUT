/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication9;

import java.awt.Color;
import javax.swing.ImageIcon;
import javax.swing.table.JTableHeader;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.StandardCopyOption;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author David
 */
public class NewJFrame extends javax.swing.JFrame {
    
    public String s;
    public String s2;

    /**
     * Creates new form NewJFrame
     */
    public NewJFrame() {
        initComponents();
        this.getContentPane().setBackground(new Color(0, 90, 158)); 

        jButton1.setIcon(new ImageIcon(this.getClass().getResource("/images/COPY.png")));
        jButton1.setRolloverIcon(new ImageIcon(this.getClass().getResource("/images/COPYH.png")));
        jButton1.setContentAreaFilled(false);
        jButton1.setFocusPainted(false);
        jButton1.setBorderPainted(false);

        jButton2.setIcon(new ImageIcon(this.getClass().getResource("/images/MOVE.png")));
        jButton2.setRolloverIcon(new ImageIcon(this.getClass().getResource("/images/MOVEH.png")));
        jButton2.setContentAreaFilled(false);
        jButton2.setFocusPainted(false);
        jButton2.setBorderPainted(false);
        
        jButton3.setIcon(new ImageIcon(this.getClass().getResource("/images/OPEN.png")));
        jButton3.setRolloverIcon(new ImageIcon(this.getClass().getResource("/images/OPENH.png")));
        jButton3.setContentAreaFilled(false);
        jButton3.setFocusPainted(false);
        jButton3.setBorderPainted(false);
        
        jButton4.setIcon(new ImageIcon(this.getClass().getResource("/images/REMOVE.png")));
        jButton4.setRolloverIcon(new ImageIcon(this.getClass().getResource("/images/REMOVEH.png")));
        jButton4.setContentAreaFilled(false);
        jButton4.setFocusPainted(false);
        jButton4.setBorderPainted(false);
        
        jButton5.setIcon(new ImageIcon(this.getClass().getResource("/images/SETTINGS.png")));
        jButton5.setRolloverIcon(new ImageIcon(this.getClass().getResource("/images/SETTINGSH.png")));
        jButton5.setContentAreaFilled(false);
        jButton5.setFocusPainted(false);
        jButton5.setBorderPainted(false);
        
        
        jCheckBox1.setIcon(new ImageIcon(this.getClass().getResource("/images/ch.png")));
        jCheckBox1.setSelectedIcon(new ImageIcon(this.getClass().getResource("/images/ch2.png")));
        jCheckBox2.setIcon(new ImageIcon(this.getClass().getResource("/images/ch.png")));
        jCheckBox2.setSelectedIcon(new ImageIcon(this.getClass().getResource("/images/ch2.png")));
        jCheckBox3.setIcon(new ImageIcon(this.getClass().getResource("/images/ch.png")));
        jCheckBox3.setSelectedIcon(new ImageIcon(this.getClass().getResource("/images/ch2.png")));
        jCheckBox4.setIcon(new ImageIcon(this.getClass().getResource("/images/ch.png")));
        jCheckBox4.setSelectedIcon(new ImageIcon(this.getClass().getResource("/images/ch2.png")));
        
        
        this.setLocationRelativeTo(null);
        s = String.valueOf(jComboBox1.getSelectedItem());
        s2 = String.valueOf(jComboBox2.getSelectedItem());
        fieldTable();
        fieldTable2();
    }
    
        public void fieldTable(){
        
        long milisec;
        Date dt;
        String type = null;

        File file = new File(s);

        File[] files = file.listFiles();
        DefaultTableModel model = (DefaultTableModel)jTable1.getModel();  
        model.setColumnIdentifiers(new String[] {"Files Names", "Type", "Date"});
        //format data
        String pattern = "yyyy-MM-dd HH:mm";
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(pattern);
        //nadřazený adresář
        
        model.addRow(new Object[]{"..","", ""});
        
        try{
            for(int i = 0; i < files.length; i++)
            {   
                if(files[i].isDirectory())
                    type = "DIR";
                if(files[i].isFile())
                    type = files[i].getName().substring(files[i].getName().lastIndexOf(".") + 1);
                milisec = files[i].lastModified();
                String date = simpleDateFormat.format(new Date(milisec));
                model.addRow(new Object[]{files[i].getName(),type, date});
            }
        }
        catch(NullPointerException ex){
                
            JOptionPane.showMessageDialog(this, "Nelze otevřít");

            String s_pom = s.substring(0, s.length()-1);
            if (s_pom.lastIndexOf("\\") == -1)
            {
                jComboBox1.setSelectedIndex(0);  
            }
            else
            {
                s = s_pom.substring(0, s_pom.lastIndexOf("\\")+1);
                removeTable();
                fieldTable();
            }

            return;
         }
        
        jLabel1.setText(s);  
    }
        
    public void fieldTable2(){
        
        long milisec;
        Date dt;
        String type = null;
        File file = new File(s2);
        File[] files = file.listFiles();
        DefaultTableModel model = (DefaultTableModel)jTable2.getModel();  
        model.setColumnIdentifiers(new String[] {"Files Names", "Type", "Date"});
        //format data
        String pattern = "yyyy-MM-dd HH:mm";
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(pattern);
        //nadřazený adresář
        
        model.addRow(new Object[]{"..","", ""});
        try{
            for(int i = 0; i < files.length; i++)
            {   
                if(files[i].isDirectory())
                    type = "DIR";
                if(files[i].isFile())
                    type = files[i].getName().substring(files[i].getName().lastIndexOf(".") + 1);
                milisec = files[i].lastModified();
                String date = simpleDateFormat.format(new Date(milisec));
                model.addRow(new Object[]{files[i].getName(),type, date});
            }
        }
               
        catch(NullPointerException ex){
                
            JOptionPane.showMessageDialog(this, "Nelze otevřít");

            String s_pom = s2.substring(0, s2.length()-1);
            if (s_pom.lastIndexOf("\\") == -1)
            {
                jComboBox2.setSelectedIndex(0);  
            }
            else
            {
                s2 = s_pom.substring(0, s_pom.lastIndexOf("\\")+1);
                removeTable2();
                fieldTable2();
            }
            return;
         }
        jLabel2.setText(s2);  
    }
    
    public void removeTable(){
        DefaultTableModel dm = (DefaultTableModel)jTable1.getModel();
        dm.getDataVector().removeAllElements();
        dm.fireTableDataChanged();
    }
    
    public void removeTable2(){
        DefaultTableModel dm = (DefaultTableModel)jTable2.getModel();
        dm.getDataVector().removeAllElements();
        dm.fireTableDataChanged();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        jTable1 = new javax.swing.JTable();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTable2 = new javax.swing.JTable();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jCheckBox1 = new javax.swing.JCheckBox();
        File[] paths;
        paths = File.listRoots();
        jComboBox1 = new javax.swing.JComboBox(paths);
        jComboBox2 = new javax.swing.JComboBox(paths);
        jButton3 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        jCheckBox2 = new javax.swing.JCheckBox();
        jCheckBox3 = new javax.swing.JCheckBox();
        jCheckBox4 = new javax.swing.JCheckBox();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("File manager");
        setBackground(new java.awt.Color(51, 255, 51));

        jTable1.setAutoCreateRowSorter(true);
        jTable1.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "name", "type", "date"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jTable1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jTable1MouseClicked(evt);
            }
        });
        jScrollPane1.setViewportView(jTable1);
        jTable1.getColumnModel().getSelectionModel().setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);

        jTable2.setAutoCreateRowSorter(true);
        jTable2.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));
        jTable2.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "name", "type", "date"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jTable2.setGridColor(new java.awt.Color(0, 90, 158));
        jTable2.setRowHeight(20);
        jTable2.setSelectionBackground(new java.awt.Color(0, 90, 158));
        jTable2.setShowVerticalLines(false);
        jTable2.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jTable2MouseClicked(evt);
            }
        });
        jScrollPane2.setViewportView(jTable2);
        jTable2.getColumnModel().getSelectionModel().setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);

        jButton1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButton1MouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButton1MouseExited(evt);
            }
        });
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jButton2.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButton2MouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButton2MouseExited(evt);
            }
        });
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        jCheckBox1.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jCheckBox1.setForeground(new java.awt.Color(255, 255, 255));
        jCheckBox1.setText("Readable");
        jCheckBox1.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        jCheckBox1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox1ActionPerformed(evt);
            }
        });

        jComboBox1.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jComboBox1.setForeground(new java.awt.Color(255, 255, 255));
        jComboBox1.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                jComboBox1ItemStateChanged(evt);
            }
        });

        jComboBox2.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jComboBox2.setForeground(new java.awt.Color(255, 255, 255));
        jComboBox2.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                jComboBox2ItemStateChanged(evt);
            }
        });

        jButton3.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButton3MouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButton3MouseExited(evt);
            }
        });
        jButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton3ActionPerformed(evt);
            }
        });

        jButton4.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButton4MouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButton4MouseExited(evt);
            }
        });
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton4ActionPerformed(evt);
            }
        });

        jButton5.setText("Settings");
        jButton5.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButton5MouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButton5MouseExited(evt);
            }
        });
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton5ActionPerformed(evt);
            }
        });

        jLabel1.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 255, 255));

        jCheckBox2.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jCheckBox2.setForeground(new java.awt.Color(255, 255, 255));
        jCheckBox2.setText("Executable");
        jCheckBox2.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        jCheckBox2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox2ActionPerformed(evt);
            }
        });

        jCheckBox3.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jCheckBox3.setForeground(new java.awt.Color(255, 255, 255));
        jCheckBox3.setText("Hidden");
        jCheckBox3.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        jCheckBox3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox3ActionPerformed(evt);
            }
        });

        jCheckBox4.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jCheckBox4.setForeground(new java.awt.Color(255, 255, 255));
        jCheckBox4.setText("Writable");
        jCheckBox4.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        jCheckBox4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheckBox4ActionPerformed(evt);
            }
        });

        jLabel2.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jLabel2.setForeground(new java.awt.Color(255, 255, 255));

        jLabel3.setFont(new java.awt.Font("Arial", 0, 14)); // NOI18N
        jLabel3.setForeground(new java.awt.Color(255, 255, 255));
        jLabel3.setText("Attributes:");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(240, 240, 240)
                .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 73, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jCheckBox1)
                .addGap(40, 40, 40)
                .addComponent(jCheckBox4)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jCheckBox2)
                .addGap(18, 18, 18)
                .addComponent(jCheckBox3)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 435, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                        .addGap(65, 65, 65)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jComboBox2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(18, 18, 18)
                                .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                            .addComponent(jScrollPane2)))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton3, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton5, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(jButton3, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(jCheckBox1)
                                .addComponent(jCheckBox2)
                                .addComponent(jCheckBox3)
                                .addComponent(jCheckBox4)))
                        .addGroup(layout.createSequentialGroup()
                            .addGap(0, 0, Short.MAX_VALUE)
                            .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jButton5, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(62, 62, 62)))
                .addGap(23, 23, 23)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jComboBox2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jComboBox1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jLabel1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        // TODO add your handling code here:
                int a;
        String b;
        File source,dest;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            source= new File(s+b);
            dest = new File(s2+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            source= new File(s2+b);
            dest = new File(s+b);
        }
        int n=0;
        if(dest.exists()){
            n = JOptionPane.showConfirmDialog(this,
                     "Chceš přepsat soubor?",
                        "Dialog",
                    JOptionPane.YES_NO_OPTION);
        }
        
        if(n==0){
            try {
                Files.copy(source.toPath(), dest.toPath(), StandardCopyOption.REPLACE_EXISTING);
            } catch (IOException ex) {
                Logger.getLogger(NewJFrame.class.getName()).log(Level.SEVERE, null, ex);
            }
            removeTable();
            fieldTable();
            removeTable2();
            fieldTable2();
        }
    }//GEN-LAST:event_jButton1ActionPerformed

    private void jButton1MouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MouseEntered
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton1MouseEntered

    private void jButton1MouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MouseExited
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton1MouseExited

    private void jButton2MouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton2MouseEntered
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton2MouseEntered

    private void jButton2MouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton2MouseExited
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton2MouseExited

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        // TODO add your handling code here:
         int a;
        String b;
        File source,dest;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            source= new File(s+b);
            dest = new File(s2+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            source= new File(s2+b);
            dest = new File(s+b);
        }
        int n=0;
        if(dest.exists()){
            n = JOptionPane.showConfirmDialog(this,
                     "Chceš přepsat soubor?",
                        "Dialog",
                    JOptionPane.YES_NO_OPTION);
        }
        
        if(n==0){
            try {
                Files.move(source.toPath(), dest.toPath(), StandardCopyOption.REPLACE_EXISTING);
            } catch (IOException ex) {
                        JOptionPane.showMessageDialog(this, "Nelze soubor umístit do vybraného adresáře");
            }
            removeTable();
            fieldTable();
            removeTable2();
            fieldTable2();
        }
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jCheckBox1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox1ActionPerformed
        // TODO add your handling code here:
                int a;
        String b;
        File f;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            f= new File(s+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            f= new File(s2+b);
        }
        
        if(jCheckBox1.isSelected()){
            if(f.setReadable(true))
                System.out.print("lldsda");
        }
        else{
            f.setReadable(false);
        }
    }//GEN-LAST:event_jCheckBox1ActionPerformed

    private void jButton3MouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton3MouseEntered
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton3MouseEntered

    private void jButton3MouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton3MouseExited
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton3MouseExited

    private void jButton3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton3ActionPerformed
        // TODO add your handling code here:
                int a;
        String b;
        File f;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            f= new File(s+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            f= new File(s2+b);
        }
        
        if(f.isFile()){
            try {
                java.awt.Desktop.getDesktop().open(f);
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Soubor nelze otevřít");
            }
        }
        else{
            JOptionPane.showMessageDialog(this, "Zvolená položka není soubor");
        }
    }//GEN-LAST:event_jButton3ActionPerformed

    private void jButton4MouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton4MouseEntered
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton4MouseEntered

    private void jButton4MouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton4MouseExited
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton4MouseExited

    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton4ActionPerformed
        // TODO add your handling code here:
        int a;
        String b;
        File f;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            f= new File(s+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            f= new File(s2+b);
        }
      
        f.delete();
        
        removeTable();
        fieldTable();
        removeTable2();
        fieldTable2();
    }//GEN-LAST:event_jButton4ActionPerformed

    private void jButton5MouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton5MouseEntered
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton5MouseEntered

    private void jButton5MouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton5MouseExited
        // TODO add your handling code here:
    }//GEN-LAST:event_jButton5MouseExited

    private void jButton5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton5ActionPerformed
        // TODO add your handling code here:
        new settings().setVisible(true);
    }//GEN-LAST:event_jButton5ActionPerformed

    private void jCheckBox2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox2ActionPerformed
        // TODO add your handling code here:
        
    }//GEN-LAST:event_jCheckBox2ActionPerformed

    private void jCheckBox3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox3ActionPerformed
        // TODO add your handling code here:
                int a;
        String b;
        File f;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            f= new File(s+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            f= new File(s2+b);
        }
        
        if(jCheckBox3.isSelected()){
            try {
                Files.setAttribute(f.toPath(), "dos:hidden", Boolean.TRUE, LinkOption.NOFOLLOW_LINKS);
            } catch (IOException ex) {
                Logger.getLogger(NewJFrame.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        else{
            try {
                Files.setAttribute(f.toPath(), "dos:hidden", Boolean.FALSE, LinkOption.NOFOLLOW_LINKS);
            } catch (IOException ex) {
                Logger.getLogger(NewJFrame.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }//GEN-LAST:event_jCheckBox3ActionPerformed

    private void jCheckBox4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheckBox4ActionPerformed
        // TODO add your handling code here:
                int a;
        String b;
        File f;
        if(jTable1.getSelectedRowCount() != 0){
            a = jTable1.getSelectedRow();
            b = jTable1.getValueAt(a, 0).toString();
            f= new File(s+b);
        }
        else{
            a = jTable2.getSelectedRow();
            b = jTable2.getValueAt(a, 0).toString();
            f= new File(s2+b);
        }
        
        if(jCheckBox4.isSelected()){
            f.setWritable(true);
        }
        else{
            f.setWritable(false);
        }
    }//GEN-LAST:event_jCheckBox4ActionPerformed

    private void jComboBox1ItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_jComboBox1ItemStateChanged
        // TODO add your handling code here:
        s = String.valueOf(jComboBox1.getSelectedItem());
        jLabel1.setText(s);
        removeTable();
        try{
            fieldTable();
        }
        catch(NullPointerException ex){
       
            JOptionPane.showMessageDialog(this, "Tento disk nelze otevřít");
            jComboBox1.setSelectedIndex(0);
        }
    }//GEN-LAST:event_jComboBox1ItemStateChanged

    private void jComboBox2ItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_jComboBox2ItemStateChanged
        // TODO add your handling code here:
        s2 = String.valueOf(jComboBox2.getSelectedItem());
        jLabel2.setText(s2);
        removeTable2();
        try{
            fieldTable2();
        }
        catch(NullPointerException ex){
       
            JOptionPane.showMessageDialog(this, "Tento disk nelze otevřít");
            jComboBox2.setSelectedIndex(0);
        }
    }//GEN-LAST:event_jComboBox2ItemStateChanged

    private void jTable1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jTable1MouseClicked
        // TODO add your handling code here:
                jTable2.clearSelection();
        int i = jTable1.getSelectedRowCount();

        String b = null;
        if(i == 1){
            int a = jTable1.getSelectedRow();
            
            b = jTable1.getValueAt(a, 0).toString();
            File newFile = new File(s+b);
            
            if(newFile.isHidden()){
                jCheckBox3.setSelected(true);
            }
            else{
                jCheckBox3.setSelected(false);
            }
            
            if(newFile.canExecute()){
                jCheckBox2.setSelected(true);
            }
            else{
                jCheckBox2.setSelected(false);
            }
            
            if(newFile.canRead()){
                jCheckBox1.setSelected(true);
            }
            else{
                jCheckBox1.setSelected(false);
            }
            
            if(newFile.canWrite()){
                jCheckBox4.setSelected(true);
            }
            else{
                jCheckBox4.setSelected(false);
            }

            if (evt.getClickCount() == 2) {
               
                
                //Otevreni souboru notepad
                if(newFile.isFile()){
                    try {
                        java.awt.Desktop.getDesktop().open(newFile);
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(this, "Soubor nelze otevřít");
                    }
                }
                
                if(newFile.isDirectory()){
                    if(a==0){
                        s = this.s.substring(0,this.s.length()-1);
                        s = this.s.substring(0,this.s.lastIndexOf("\\")+1);
                    }else
                        s = s + b +"\\";
                    
                    removeTable();
                    try{
                        fieldTable();
                    }
                    catch (Exception ex)
                    {
                        s = String.valueOf(jComboBox1.getSelectedItem());
                        jComboBox1.setSelectedIndex(jComboBox1.getSelectedIndex());
                        removeTable();
                        fieldTable();
                    }
                    
                }
            }
        }
        else
            jLabel1.setText("");
    }//GEN-LAST:event_jTable1MouseClicked

    private void jTable2MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jTable2MouseClicked
        // TODO add your handling code here:
         jTable1.clearSelection();
        
        int i = jTable2.getSelectedRowCount();

        String b = null;
        if(i == 1){
            int a = jTable2.getSelectedRow();
            
            b = jTable2.getValueAt(a, 0).toString();
            File newFile = new File(s2+b);
            
            if(newFile.isHidden()){
                jCheckBox3.setSelected(true);
            }
            else{
                jCheckBox3.setSelected(false);
            }
            
            if(newFile.canExecute()){
                jCheckBox2.setSelected(true);
            }
            else{
                jCheckBox2.setSelected(false);
            }
            
            if(newFile.canRead()){
                jCheckBox1.setSelected(true);
            }
            else{
                jCheckBox1.setSelected(false);
            }
            
            if(newFile.canWrite()){
                jCheckBox4.setSelected(true);
            }
            else{
                jCheckBox4.setSelected(false);
            }

            if (evt.getClickCount() == 2) {
                
                //Otevreni souboru notepad
                if(newFile.isFile()){
                    try {
                        java.awt.Desktop.getDesktop().open(newFile);
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(this, "Soubor nelze otevřít");
                    }
                }
                
                if(newFile.isDirectory()){
                    if(a==0){
                        s2 = this.s2.substring(0,this.s2.length()-1);
                        s2 = this.s2.substring(0,this.s2.lastIndexOf("\\")+1);
                    }else
                        s2 = s2 + b +"\\";
                    
                    removeTable2();
                    try{
                        fieldTable2();
                    }
                    catch (Exception ex)
                    {
                        s2 = String.valueOf(jComboBox2.getSelectedItem());
                        jComboBox2.setSelectedIndex(jComboBox2.getSelectedIndex());
                        removeTable2();
                        fieldTable2();
                    }
                    
                }
            }
        }
        else
            jLabel2.setText("");
    }//GEN-LAST:event_jTable2MouseClicked

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(NewJFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new NewJFrame().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JCheckBox jCheckBox1;
    private javax.swing.JCheckBox jCheckBox2;
    private javax.swing.JCheckBox jCheckBox3;
    private javax.swing.JCheckBox jCheckBox4;
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JComboBox<String> jComboBox2;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTable jTable1;
    private javax.swing.JTable jTable2;
    // End of variables declaration//GEN-END:variables
}
