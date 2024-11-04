import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Random;
import javax.swing.Timer;
import javax.sound.sampled.*;
import javax.swing.ImageIcon;
import javax.swing.JPanel;

public class FlappyBird extends JPanel implements ActionListener,KeyListener{
     int boardWidth=360;
     int boardHeight=640;
        //Images
        Image backgroundImg;
        Image birdImg;
        Image toppipeImg;
        Image bottompipeImg;

        //Bird
        int birdX=boardWidth/8;
        int birdY=boardHeight/2;
        int birdWidth=34;
        int birdHeight=24;

        class Bird
        {
            int x=birdX;
            int y=birdY;
            int width=birdWidth;
            int height=birdHeight; 
            Image img;

            Bird(Image img)
            {
                this.img=img;
            }
        }

        //Pipes
        int pipeX=boardWidth;
        int pipeY=0;
        int pipeWidth=64;
        int pipeHeight=512; 

        class Pipe{
            int x=pipeX;
            int y=pipeY;
            int width=pipeWidth;
            int height=pipeHeight;
            Image img;
            boolean passed=false;
            Pipe(Image img)
            {
                this.img=img;
            }
        }

        //game logic
        Bird bird;
        int velocityX=-4;
        int velocityY=0; 
        int gravity=1;

        ArrayList<Pipe> pipes;
        Random random=new Random();

        Timer gameLoop;
        Timer  placePipesTimer;

        boolean gameOver=false;
        double score=0;
        
        private Clip gameOverSound;
        private Clip jumpSound;

        FlappyBird(){
        setPreferredSize(new Dimension(boardWidth, boardHeight));
        // setBackground(Color.BLUE); 
        setFocusable(true);
        addKeyListener(this);
        
        //load Images
        backgroundImg=new ImageIcon(getClass().getResource("./flappybirdbg.png")).getImage();
        birdImg=new ImageIcon(getClass().getResource("./flappybird.png")).getImage();
        toppipeImg=new ImageIcon(getClass().getResource("./toppipe.png")).getImage();
        bottompipeImg=new ImageIcon(getClass().getResource("./bottompipe.png")).getImage();
        
        //bird
        bird=new Bird(birdImg);
        pipes=new ArrayList<Pipe>();

        //place pipes Timers
        placePipesTimer=new Timer(1500,new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                placePipes(); 
            }
        });

        //pipeTimer
        placePipesTimer.start();
         
        //game timer
        gameLoop=new Timer(1000/38,this);
        gameLoop.start();
         
        //gameOver sound
        try {
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(getClass().getResource("./gameover.wav"));
            gameOverSound = AudioSystem.getClip();
            gameOverSound.open(audioInputStream);
        } catch (Exception e) {
            e.printStackTrace();
        }

        //jump sound

        try {
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(getClass().getResource("./jump.wav"));
            jumpSound = AudioSystem.getClip();
            jumpSound.open(audioInputStream);
        } catch (Exception e) {
            e.printStackTrace();
        }
    
     }
     
    public void placePipes()
    {

        // Random random = new Random();
        // int gap = 120;
        // int pipeY = random.nextInt(boardHeight - gap - pipeHeight);

        int randomPipeY=(int)(pipeY-pipeHeight/4-Math.random()*(pipeHeight/2));
        int openingSpace=boardHeight/4;

        Pipe topPipe=new Pipe(toppipeImg);
        topPipe.y=randomPipeY;
        topPipe.x=pipeX;
        pipes.add(topPipe);

        Pipe bottomPipe=new Pipe(bottompipeImg);
        bottomPipe.y=topPipe.y + pipeHeight + openingSpace;
        pipes.add(bottomPipe);

        // Pipe bottomPipe=new Pipe(bottompipeImg);
        // bottomPipe.y=pipeY+gap;
        // bottomPipe.x=pipeX;
        // pipes.add(bottomPipe);
    }

     public void paintComponent(Graphics g)
     {
        super.paintComponent(g);
        draw(g);
     }


     public void draw(Graphics g) 
     {
        // System.out.println("draw"); 
        //background
        g.drawImage(backgroundImg, 0, 0, boardWidth,boardHeight,null);
        
        //bird
        g.drawImage(bird.img, bird.x, bird.y, bird.width, bird.height, null);
         
        //pipes
        for (int i = 0; i < pipes.size(); i++) {
            Pipe pipe = pipes.get(i);
            g.drawImage(pipe.img,pipe.x,pipe.y,pipe.width,pipe.height,null);
        }

        g.setColor(Color.WHITE);
        g.setFont(new Font("Arial",Font.PLAIN,32));

        if(gameOver)
        {
            

            String gameOverMessage = "GAME OVER!!";
        String scoreMessage = "Your Score is: " + String.valueOf((int) score);
        String restartMessage = "Press Space to Restart";

        // Create a list of fonts and colors
        Font[] fonts = new Font[] {
            new Font("Arial", Font.BOLD, 32),
            new Font("Comic Sans MS", Font.BOLD, 32),
            new Font("Impact", Font.BOLD, 32),
            new Font("Tahoma", Font.BOLD, 32)
        };

        Color[] colors = new Color[] {
            Color.RED,
            Color.GREEN,
            Color.BLUE,
            Color.YELLOW,
            Color.CYAN,
            Color.MAGENTA
        };

        // Draw a black box behind the messages
        g.setColor(Color.BLACK);
        g.fillRect((boardWidth - 250) / 2, (boardHeight - 150) / 2, 250, 150);

        // Draw the game over message
        int x = (boardWidth - g.getFontMetrics(fonts[0]).stringWidth(gameOverMessage)) / 2;
        int y = (boardHeight - g.getFontMetrics(fonts[0]).getHeight()) / 2;
        for (int i = 0; i < gameOverMessage.length(); i++) {
            g.setFont(fonts[i % fonts.length]);
            g.setColor(colors[i % colors.length]);
            g.drawString(gameOverMessage.substring(i, i + 1), x, y);
            x += g.getFontMetrics().charWidth(gameOverMessage.charAt(i));
        }

        // Draw the score message
        x = (boardWidth - g.getFontMetrics(fonts[0]).stringWidth(scoreMessage)) / 2;
        y += g.getFontMetrics(fonts[0]).getHeight();
        for (int i = 0; i < scoreMessage.length(); i++) {
            g.setFont(fonts[i % fonts.length]);
            g.setColor(colors[i % colors.length]);
            g.drawString(scoreMessage.substring(i, i + 1), x, y);
            x += g.getFontMetrics().charWidth(scoreMessage.charAt(i));
        }

        // // Draw the restart message
        // x = (boardWidth - g.getFontMetrics(fonts[0]).stringWidth(restartMessage)) / 2;
        // y += g.getFontMetrics(fonts[0]).getHeight();
        // for (int i = 0; i < restartMessage.length(); i++) {
        //     g.setFont(fonts[i % fonts.length]);
        //     g.setColor(colors[i % colors.length]);
        //     g.drawString(restartMessage.substring(i, i + 1), x, y);
        //     x += g.getFontMetrics().charWidth(restartMessage.charAt(i));
        // }

            // g.drawString("Game Over: " + String.valueOf((int)score),10,35);
            gameOverSound.start();
            gameOverSound.setFramePosition(0);

        }

        else
        {
            g.drawString("Score: " + String.valueOf((int)score),10,35);
        }
    }

    public void move()
    {
        //bird
        velocityY+=gravity;
        bird.y+=velocityY;
        bird.y=Math.max(bird.y,0); 

        //pipes
        for (int i = 0; i < pipes.size(); i++) 
        {
          Pipe pipe=pipes.get(i);
          pipe.x+=velocityX;
          
          if(!pipe.passed && bird.x > pipe.x +pipe.width)
          {
            pipe.passed=true;
            score += 0.5;
          }

          if(collision(bird, pipe)){
            gameOver=true;
          }
        }

        if(bird.y>boardHeight)
        {
            gameOver=true;
        }
    }
    

    public boolean collision(Bird a, Pipe b)
     {
        return a.x < b.x + b.width &&
               a.x + a.width > b.x &&
               a.y < b.y + b.height &&
               a.y + a.height > b.y ;
     }

    @Override 
    public void actionPerformed(ActionEvent e) {
        move();
        repaint();
        if(gameOver)
        {
            placePipesTimer.stop();
            gameLoop.stop();

        }
    }
   
    @Override
    public void keyPressed(KeyEvent e) {
       if(e.getKeyCode()==KeyEvent.VK_SPACE){
        velocityY=-9;
        jumpSound.start();
        jumpSound.setFramePosition(0);
        if(gameOver)
        {
            //restart the game
            bird.y=birdY;
            velocityY=0;
            pipes.clear();
            score=0;
            gameOver=false;
            gameLoop.start(); 
            placePipesTimer.start();
        }
       
            
       }
    }
    @Override
    public void keyTyped(KeyEvent e) {
       
    }
    @Override
    public void keyReleased(KeyEvent e) {
    }

    
}
