import wx
import os
import serial.tools.list_ports
from lib.mcu_control import MCUControl

class MainWindow(wx.Frame):
    def __init__(self, parent, title):

        self.mcu = MCUControl()

        self.dirname=''

        # A "-1" in the size parameter instructs wxWidgets to use the default size.
        # In this case, we select 200px width and the default height.
        wx.Frame.__init__(self, parent, title=title, size=(600,-1))
        self.control = wx.TextCtrl(self, style=wx.TE_MULTILINE)
        self.CreateStatusBar() # A Statusbar in the bottom of the window

        # Setting up the menu.
        filemenu= wx.Menu()
        menuOpen = filemenu.Append(wx.ID_OPEN, "&Open"," Open a file to edit")
        menuAbout= filemenu.Append(wx.ID_ABOUT, "&About"," Information about this program")
        menuExit = filemenu.Append(wx.ID_EXIT,"E&xit"," Terminate the program")

        # Creating the menubar.
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File") # Adding the "filemenu" to the MenuBar
        self.SetMenuBar(menuBar)  # Adding the MenuBar to the Frame content.

        # Events.
        self.Bind(wx.EVT_MENU, self.OnOpen, menuOpen)
        self.Bind(wx.EVT_MENU, self.OnExit, menuExit)
        self.Bind(wx.EVT_MENU, self.OnAbout, menuAbout)

        self.sizer2 = wx.BoxSizer(wx.HORIZONTAL)
        self.buttons = []

        self.upload_button = wx.Button(self, -1, "Upload")
        self.upload_button.Bind(wx.EVT_BUTTON, self.OnUpload)

        self.serial_selector = self.getSerialChoiceWidget()

        self.mcu_selector = wx.Choice(self, -1, choices=["0","1","2"])

        self.sizer2.AddSpacer(5);
        self.sizer2.Add(wx.StaticText(self, -1, label="Serialport: "), 1, wx.TOP|wx.BOTTOM|wx.ALIGN_CENTER_VERTICAL)
        self.sizer2.Add(self.serial_selector, 2, wx.TOP|wx.BOTTOM|wx.ALIGN_CENTER_VERTICAL, 2)
        self.sizer2.AddSpacer(5);
        self.sizer2.Add(wx.StaticText(self, -1, label="MCU ID: "), 3, wx.TOP|wx.BOTTOM|wx.ALIGN_CENTER_VERTICAL)
        self.sizer2.Add(self.mcu_selector, 4, wx.TOP|wx.BOTTOM|wx.ALIGN_CENTER_VERTICAL, 2)
        self.sizer2.AddSpacer(5);
        self.sizer2.Add(self.upload_button, 5)
        self.sizer2.AddSpacer(5);


        # Use some sizers to see layout options
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.control, 1, wx.EXPAND)
        self.sizer.Add(self.sizer2, 0, wx.EXPAND)

        #Layout sizers
        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        #self.sizer.Fit(self)
        self.Show()

    def OnAbout(self,e):
        # Create a message dialog box
        dlg = wx.MessageDialog(self, " An IDE for the MCxxxx series of processors", "", wx.OK)
        dlg.ShowModal() # Shows it
        dlg.Destroy() # finally destroy it when finished.

    def OnExit(self,e):
        self.Close(True)  # Close the frame.

    def OnOpen(self,e):
        """ Open a file"""
        dlg = wx.FileDialog(self, "Choose a file", self.dirname, "", "*.*", wx.FD_OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname, self.filename), 'r')
            self.control.SetValue(f.read())
            f.close()
        dlg.Destroy()

    def getSerialChoiceWidget(self):
        ports = []
        default = 0
        for i, port in enumerate(serial.tools.list_ports.comports()):
            ports.append(port[0])
            if "usbserial" in port[0]:
                default = i
        widget = wx.Choice(self, -1, choices=ports)
        widget.SetSelection(default)
        return widget

    def OnUpload(self,e):
        code = self.control.GetValue()
        serial_port = self.serial_selector.GetString(self.serial_selector.GetSelection())
        mcu_id = self.mcu_selector.GetString(self.mcu_selector.GetSelection())
        self.mcu.UploadCode(code, mcu_id, serial_port)


app = wx.App(False)
frame = MainWindow(None, "MCxxxx IDE V0.1 pre-alpha")
app.MainLoop()
