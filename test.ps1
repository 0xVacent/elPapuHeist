Add-Type -AssemblyName PresentationCore

$global:MediaPlayer = New-Object System.Windows.Media.MediaPlayer

function Play-Sound {
    param (
        [string]$SoundFilePath
    )

    $MediaPlayer.Open([System.Uri]($SoundFilePath))
    $MediaPlayer.Play()
}

function Stop-Sound {
    $MediaPlayer.Stop()
}