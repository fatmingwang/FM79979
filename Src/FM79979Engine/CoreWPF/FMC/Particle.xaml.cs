using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CoreWPF.FMC
{
    /// <summary>
    /// Interaction logic for Particle.xaml
    /// </summary>
    public partial class Particle : UserControl
    {
        public event EventHandler f_AddParticle;
        public event EventHandler f_DelParticle;
        public event EventHandler f_ParticleChangeSelection;
        public event EventHandler f_ParticleGroupChangeSelection;
        public event EventHandler f_ParticleGroupListChangeSelection;

        public Particle()
        {
            InitializeComponent();
        }

        private void AddParticle_Button_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddParticle != null)
            {
                f_AddParticle(sender, e);
            }
        }

        private void DelParticle_Button_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelParticle != null)
            {
                f_DelParticle(sender, e);
            }
        }

        private void Particle_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (f_ParticleChangeSelection != null)
            {
                f_ParticleChangeSelection(sender, e);
            }
        }

        private void ParticleGroupList_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (f_ParticleGroupListChangeSelection != null)
            {
                f_ParticleGroupListChangeSelection(sender, e);
            }
        }

        private void ParticleGroup_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (f_ParticleGroupChangeSelection != null)
            {
                f_ParticleGroupChangeSelection(sender, e);
            }
        }

        private void m_Particle_listBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            object item = m_Particle_listBox.SelectedItem;
            if (item != null)
                DragDrop.DoDragDrop(m_Particle_listBox, item, DragDropEffects.Move);
        }
    }
}
